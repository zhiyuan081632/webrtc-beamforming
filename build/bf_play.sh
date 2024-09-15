# ADC1: MIC1
tinymix set "ADC1 Input FMINL Switch" 0
tinymix set "ADC1 Input MIC1 Boost Switch" 1
tinymix set "ADC1 Input LINEINL Switch" 0

# ADC2: LINEINR
tinymix set "ADC2 Input FMINR Switch" 0
tinymix set "ADC2 Input MIC2 Boost Switch" 0
tinymix set "ADC2 Input LINEINR Switch" 1

# ADC3: MIC2
tinymix set "ADC3 Input MIC3 Boost Switch" 1

tinymix set "MIC1 Input Select" 0
tinymix set "MIC2 Input Select" 0
tinymix set "MIC3 Input Select" 0

# Speaker: LINEOUT
tinymix set "HpSpeaker Switch" 1
tinymix set "LINEOUT Switch" 0
tinymix set "Headphone Switch" 1
tinymix set "Headphone volume" 7

# algorithm
export LD_LIBRARY_PATH=./lib
chmod 777 bf_2mic
./bf_2mic 0 1
