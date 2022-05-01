../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 \
  -p build/tests/userprog/comb-ewcorwc -a comb-ewcorwc \
  -p build/tests/userprog/child-corwc  -a child-corwc \
  -- -q  -f run comb-ewcorwc