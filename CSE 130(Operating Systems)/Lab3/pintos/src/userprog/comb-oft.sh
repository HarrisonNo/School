../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 \
  -p build/tests/userprog/comb-oft -a comb-oft \
  -p build/tests/userprog/child-oft -a child-oft \
  -- -q  -f run comb-oft