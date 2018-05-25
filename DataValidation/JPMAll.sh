#!/bin/bash

cd ../PositionMapping/Cpp_Correction/

for luxstamp in 20141006 20150226 20150722 20160222; do
    ./JacobPositionMagic 1 0 ../../DataValidation/Uniform_${luxstamp}.txt "off" ${luxstamp}
done

cd ../../DataValidation/

