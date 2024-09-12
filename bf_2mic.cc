#include <tinyalsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <libgen.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/select.h>
#include <linux/input.h>
#include <thread>

#include <vector>

#include "gflags/gflags.h"
#include "webrtc/base/checks.h"
#include "webrtc/base/format_macros.h"
#include "webrtc/common_audio/channel_buffer.h"
#include "webrtc/common_audio/wav_file.h"
#include "webrtc/modules/audio_processing/beamformer/nonlinear_beamformer.h"
#include "webrtc/modules/audio_processing/test/test_utils.h"
#include <time.h>


#define PCM_CARD 0
#define PCM_DEVICE 0
#define PCM_RATE 16000
#define PCM_BITS 16
#define PCM_CHANNELS_IN 3 //mic:1, line:2, 2mic+line:3(format: mic1, line, mic2)
#define PCM_CHANNELS_OUT 2 //headphone:2

#define PCM_FRAMES_ALG 160 // frames of alg process

#define PCM_FRAMES_IN PCM_FRAMES_ALG 
#define PCM_COUNT_IN 4
#define PCM_FRAMES_OUT PCM_FRAMES_ALG
#define PCM_COUNT_OUT 4


#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1

namespace webrtc {

struct wav_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t riff_fmt;
    uint32_t fmt_id;
    uint32_t fmt_sz;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_id;
    uint32_t data_sz;
};

int g_running = 1;
int prinfo = 1;
int run_mode = 1;  // 0: Bypass, 1: ANS, 2: ANS simulation
int save_mode = 0; // 0: not save, 1: save

#define INADEQUATE_CONDITIONS 3
typedef enum { KEY_CODE_NONE = 0, KEY_CODE_USER0, KEY_CODE_USER1 } KeyCode;

// SoundConnect *sc_cap;
#define SC_CONF_ANS "conf/bf_3ch.conf"

void sigint_handler(int sig)
{
    if (sig == SIGINT){
        g_running = 0;
    }
}

static int check_button_pressed(int fd) {
    assert(fd >= 0);

    /* wait button being pressed or released. */
    fd_set input;
    FD_ZERO(&input);
    FD_SET(fd, &input);
    int ret = select(fd + 1, &input, NULL, NULL, NULL);
    if (ret < 0) {
        printf("%s", strerror(errno));
        return -1;
    }

    /* read event */
    struct input_event buf;
    if (read(fd, &buf, sizeof(struct input_event)) < 0) {
        printf("%s", strerror(errno));
        return -1;
    }

    /* Check the input_event value */
    switch (buf.code) {
    case KEY_PROG1:
        /* 1: pressed; 0: released */
        if (buf.value == 1) {
            return KEY_CODE_USER0;
        }
        break;
    case KEY_PROG2:
        if (buf.value == 1)
            return KEY_CODE_USER1;
        break;
    default:
        return KEY_CODE_NONE;
        break;
    }

    return KEY_CODE_NONE;
}

void* check_button_thread(void* arg) {
    int fd = *(int*)arg;
    while (g_running) {
        int key_code = check_button_pressed(fd);

        if (key_code < 0)
            continue;
        printf("key_code = %d\n", key_code);
        switch (key_code) {
            case KEY_CODE_USER0:
            case KEY_CODE_USER1:
                printf("User key pressed!\n");
                // Toggle run_mode
                if(run_mode == 0) {
                    run_mode = 1;
                    printf("run_mode: %d\n", run_mode);
                    printf("ANS process\n");
                    // Turn on user LED
                    system("echo 1 > /sys/class/leds/user-led0/brightness");
                }
                else {
                    run_mode = 0;
                    printf("run_mode: %d\n", run_mode);
                    printf("Bypass\n");
                    // Turn off user LED
                    system("echo 0 > /sys/class/leds/user-led0/brightness");
                }
                break;
            default:
                break;
        }        
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    struct pcm_config config_record;
    struct pcm_config config_play;
    struct pcm *pcm_in;
    struct pcm *pcm_out;
    char *buff_record;
    char *buff_play;
    char *buff_alg;
    struct wav_header header_record;
    struct wav_header header_play;
    const char *fname_record = "record.wav";
    const char *fname_play = "play.wav";
    FILE *fp_record=NULL;
    FILE *fp_play=NULL;
    enum pcm_format format;
    int no_header = 0;
    
    int size_record;
    int size_alg;
    int size_play;
    int bytes_per_frame_record;
    int bytes_per_frame_alg;
    int bytes_per_frame_play;
    int total_frames_record;
    int frames_record;
    int frames_play;

    // printf("alg version: %s\n", SoundConnect::version());
    printf("app build: %s, %s\n", __DATE__, __TIME__);
    if (argc == 1) {
        run_mode = 0;
        save_mode = 0;
    }
    else if (argc == 2) {
        run_mode = atoi(argv[1]);
        save_mode = 0;
    }
    else if (argc == 3) {
        run_mode = atoi(argv[1]);
        save_mode = atoi(argv[2]);
    }
    else {
        printf("Usage: %s <run_mode> [save_mode]\n", argv[0]);
        printf("run_mode: 0: Bypass, 1: ANS, 2: ANS simulation\n");
        printf("save_mode: 0: no save, 1: save\n");
        return 0;
    }
    printf("run_mode: %d\n", run_mode);
    printf("save_mode: %d\n", save_mode);

    // Configure the PCM for record
    memset(&config_record, 0, sizeof(config_record));
    config_record.channels = PCM_CHANNELS_IN;
    config_record.rate = PCM_RATE;
    config_record.period_size = PCM_FRAMES_IN;
    config_record.period_count = PCM_COUNT_IN;
    config_record.format = PCM_FORMAT_S16_LE;
    config_record.start_threshold = 0; 
    config_record.stop_threshold = 0; 
    config_record.silence_threshold = 0;

    // Open the PCM device in capture mode
    pcm_in = pcm_open(PCM_CARD, PCM_DEVICE, PCM_IN, &config_record);
    if (!pcm_is_ready(pcm_in)) {
        printf("Error opening PCM device %u PCM_IN(%s)\n", PCM_DEVICE, pcm_get_error(pcm_in));
        return -1;
    }
    printf("Recrod device ready\n");

    // Configure the PCM for play
    memset(&config_play, 0, sizeof(config_play));
    config_play.channels = PCM_CHANNELS_OUT;
    config_play.rate = PCM_RATE;
    config_play.period_size = PCM_FRAMES_OUT;
    config_play.period_count = PCM_COUNT_OUT;
    config_play.format = PCM_FORMAT_S16_LE;
    config_play.start_threshold = PCM_FRAMES_OUT;
    config_play.stop_threshold = PCM_FRAMES_OUT * PCM_COUNT_OUT;
    config_play.silence_threshold = PCM_FRAMES_OUT * PCM_COUNT_OUT;
    // Open the PCM device in playback mode
    pcm_out = pcm_open(PCM_CARD, PCM_DEVICE, PCM_OUT, &config_play);
    if (!pcm_is_ready(pcm_out)) {
        printf("Error opening PCM device %u (%s)\n", PCM_DEVICE, pcm_get_error(pcm_out));
        return -1;
    }
    printf("Play device ready\n");

    // Initialize alg
#if 0
    // ANS
    sc_cap = new SoundConnect(SC_CONF_ANS);
    if (!sc_cap) {
        printf("SoundConnect init failed\n");
        return -1;
    }

    // Allocate the buffer for alg
    size_alg = sc_cap->bytesPerBlockOut(); //960 bytes
    buff_alg = (char*)malloc(size_alg);
    if (!buff_alg) {
        fprintf(stderr, "Unable to allocate %u bytes for buff_alg\n", size_alg);
        return 0;
    }
    printf("alg buffer size: %d\n", size_alg);
    memset(buff_alg, 0, size_alg);
#else

    // BF
    const int kChunksPerSecond = 100;
    const int kChunkSizeMs = 1000 / kChunksPerSecond;

    std::vector<Point> array_geometry = {
        {-0.025, 0, 0},
        {+0.025, 0, 0}
    };
    const size_t num_mics = 2;

    NonlinearBeamformer bf(array_geometry);
    bf.Initialize(kChunkSizeMs, PCM_RATE);

    ChannelBuffer<float> in_buf(rtc::CheckedDivExact(16000, kChunksPerSecond), num_mics);
    ChannelBuffer<float> out_buf(rtc::CheckedDivExact(16000, kChunksPerSecond), PCM_CHANNELS_OUT);
    std::vector<float> interleaved(in_buf.size());

#endif
    
    printf("Init alg succeed\n");

    // Allocate the buffer for record
    size_record = pcm_frames_to_bytes(pcm_in, PCM_FRAMES_ALG);
    buff_record = (char*)malloc(size_record);
    if (!buff_record) {
        fprintf(stderr, "Unable to allocate %u bytes for record\n", size_record);
        pcm_close(pcm_in);
        return 0;
    }
    printf("record buffer size: %d\n", size_record);
    memset(buff_record, 0, size_record);

    // Allocate the buffer for play
    size_play = pcm_frames_to_bytes(pcm_out, PCM_FRAMES_ALG);
    buff_play = (char*)malloc(size_play);
    if (!buff_play) {
        fprintf(stderr, "Unable to allocate %u bytes for buff_play\n", size_play);
        pcm_close(pcm_out);
        return 0;
    }
    printf("play buffer size: %d\n", size_play);
    memset(buff_play, 0, size_play);

    bytes_per_frame_record = pcm_frames_to_bytes(pcm_in, 1);
    printf("bytes_per_frame_record: %d\n", bytes_per_frame_record);
    bytes_per_frame_alg = 1 * (PCM_BITS / 8); // 1 channel, 16 bits
    printf("bytes_per_frame_alg: %d\n", bytes_per_frame_alg);
    bytes_per_frame_play = pcm_frames_to_bytes(pcm_out, 1);
    printf("bytes_per_frame_play: %d\n", bytes_per_frame_play);
    total_frames_record = 0;
    frames_record = 0;

    // Save the record and play data to file
    if(save_mode == 1) {
        switch (PCM_BITS) {
        case 32:
            format = PCM_FORMAT_S32_LE;
            break;
        case 24:
            format = PCM_FORMAT_S24_LE;
            break;
        case 16:
            format = PCM_FORMAT_S16_LE;
            break;
        default:
            fprintf(stderr, "%u bits is not supported.\n", PCM_BITS);
            return 1;
        }
        printf("format: %d\n", format);

        // Initialize wav header for record
        header_record.riff_id = ID_RIFF;
        header_record.riff_sz = 0;
        header_record.riff_fmt = ID_WAVE;
        header_record.fmt_id = ID_FMT;
        header_record.fmt_sz = PCM_BITS;
        header_record.audio_format = FORMAT_PCM;
        header_record.num_channels = PCM_CHANNELS_IN;
        header_record.sample_rate = PCM_RATE;
        header_record.bits_per_sample = pcm_format_to_bits(format);
        header_record.byte_rate = (header_record.bits_per_sample / 8) * PCM_CHANNELS_IN * PCM_RATE;
        header_record.block_align = PCM_CHANNELS_IN * (header_record.bits_per_sample / 8);
        header_record.data_id = ID_DATA;

        // Open the record file
        fp_record = fopen(fname_record, "wb");
        if (!fp_record) {
            fprintf(stderr, "Unable to create file '%s'\n", fname_record);
            return 1;
        }
        printf("Create record file '%s'\n", fname_record);

        // Leave enough room for header
        if (!no_header) {
            fseek(fp_record, sizeof(struct wav_header), SEEK_SET);
        }

        // Initialize wav header for play
        header_play.riff_id = ID_RIFF;
        header_play.riff_sz = 0;
        header_play.riff_fmt = ID_WAVE;
        header_play.fmt_id = ID_FMT;
        header_play.fmt_sz = PCM_BITS;
        header_play.audio_format = FORMAT_PCM;
        header_play.num_channels = PCM_CHANNELS_OUT;
        header_play.sample_rate = PCM_RATE;
        header_play.bits_per_sample = pcm_format_to_bits(format);
        header_play.byte_rate = (header_play.bits_per_sample / 8) * PCM_CHANNELS_OUT * PCM_RATE;
        header_play.block_align = PCM_CHANNELS_OUT * (header_play.bits_per_sample / 8);
        header_play.data_id = ID_DATA;

        // Open the play file
        fp_play = fopen(fname_play, "wb");
        if (!fp_play) {
            fprintf(stderr, "Unable to create file '%s'\n", fname_play);
            return 1;
        }
        printf("Create alg file '%s'\n", fname_play);

        if (!no_header) {
            fseek(fp_play, sizeof(struct wav_header), SEEK_SET);
        }
    }

    // User LED
    if (run_mode == 0) {
        // Turn off user LED
        system("echo 0 > /sys/class/leds/user-led0/brightness");
    }
    else {
        // Turn on user LED
        system("echo 1 > /sys/class/leds/user-led0/brightness");
    }
    
    // User key
    const char *dev = "/dev/input/event0"; //user key device
    int fd = open(dev, O_RDONLY);
    if (fd < 0) {
        printf("Error: Failed to open device\n");
        return INADEQUATE_CONDITIONS;
    }
    printf("Init User key: KEY2.\n");
    
    pthread_t checkThread;
    if (pthread_create(&checkThread, NULL, check_button_thread, &fd) != 0) {
        printf("Failed to create check button thread\n");
        return -1;
    }

    // install signal handler and begin capturing
    signal(SIGINT, sigint_handler);

    // Read from line-in and write to headphones
    while (g_running) {
        // Read the data from 2mic+line-in-R
        frames_record = pcm_readi(pcm_in, buff_record, PCM_FRAMES_IN);
        // printf("frames_record: %d\n", frames_record);
        total_frames_record += frames_record;

        // process with different mode
        if (run_mode == 0) {
            // Copy mic1 to line-out-L, line-in-R to line-out-R
            for (int i = 0, j=0; j < size_record; i += bytes_per_frame_play, j += bytes_per_frame_record) { 
                memcpy(buff_play + i, buff_record + j, 2); // mic1 to line-out-L
                memcpy(buff_play + i + 2, buff_record + j + 2, 2); // line-in-R to line-out-R
            }
        } 
        else if(run_mode == 1) {

            // copy to interleaved buffer
            size_t num_samples = size_record / 6; 
            for (size_t i = 0, j = 0; i < size_record && j < num_samples*2; i += 6, j += 2) {
                // 提取第1和3通道
                int16_t sample1 = *reinterpret_cast<int16_t*>(&buff_record[i]);
                int16_t sample3 = *reinterpret_cast<int16_t*>(&buff_record[i + 4]);

                // 将提取的样本保存到interleaved中
                interleaved[j] = sample1;
                interleaved[j + 1] = sample3;
            }

            FloatS16ToFloat(&interleaved[0], interleaved.size(), &interleaved[0]);
            Deinterleave(&interleaved[0], in_buf.num_frames(), in_buf.num_channels(), in_buf.channels());

            // process
            bf.ProcessChunk(in_buf, &out_buf);

            Interleave(out_buf.channels(), out_buf.num_frames(), out_buf.num_channels(), &interleaved[0]);
            FloatToFloatS16(&interleaved[0], interleaved.size(), &interleaved[0]);

            // copy to play buffer
            size_t size_play_needed = num_samples * 4; 
            for (size_t i = 0, j = 0; i < size_play_needed; i += 4, j += 2) {
                // 假设buff_play是char类型，并且每个样本需要2字节（即16位）
                reinterpret_cast<int16_t*>(buff_play)[i/2] = interleaved[j];
                reinterpret_cast<int16_t*>(buff_play)[(i/2) + 1] = interleaved[j+1];
            }
        }

        // write it to the headphones
        frames_play = pcm_writei(pcm_out, buff_play, PCM_FRAMES_OUT); 
        // printf("frames_play: %d\n", frames_play);   
        if (frames_play <= 0) {
            printf("Error playing to PCM device\n");
            //break;
        }

        if (save_mode == 1) {
            // Write the data to the record file
            size_t re_record = fwrite(buff_record, bytes_per_frame_record, size_record/bytes_per_frame_record, fp_record);
            if (re_record != (size_t)frames_record) {
                fprintf(stderr, "Error write record file\n");
                break;
            }
            // Write the data to the play file
            size_t re_play = fwrite(buff_play, bytes_per_frame_play, size_play/bytes_per_frame_play, fp_play);
            if (re_play != (size_t)frames_play) {
                fprintf(stderr, "Error write play file\n");
                // break;
            }
        }
    }


    if (prinfo) {
        printf("Record %u frames, %.3f seconds\n", total_frames_record, (double)total_frames_record / PCM_RATE);
    }

    if (save_mode == 1) {
        // write record wav header now all information is known
        if (!no_header) {
            header_record.data_sz = total_frames_record * header_record.block_align;
            header_record.riff_sz = header_record.data_sz + sizeof(header_record) - 8;
            fseek(fp_record, 0, SEEK_SET);
            fwrite(&header_record, sizeof(struct wav_header), 1, fp_record);
        }
        // write play wav header now all information is known
        if (!no_header) {
            header_play.data_sz = total_frames_record * header_play.block_align;
            header_play.riff_sz = header_play.data_sz + sizeof(header_play) - 8;
            fseek(fp_play, 0, SEEK_SET);
            fwrite(&header_play, sizeof(struct wav_header), 1, fp_play);
        }
        // Close the file
        if (fp_record != NULL) {
            fclose(fp_play);
        }
        if (fp_play != NULL) {  
            fclose(fp_play);
        }
    }

    pthread_join(checkThread, NULL);
    close(fd);

  #if 0
    delete sc_cap;
  #else

  #endif

    // Clean up
    free(buff_record);
    // free(buff_alg);
    free(buff_play);
    pcm_close(pcm_in);
    pcm_close(pcm_out);

    return 0;
}

}  // namespace webrtc

int main(int argc, char* argv[]) {
  return webrtc::main(argc, argv);
}
