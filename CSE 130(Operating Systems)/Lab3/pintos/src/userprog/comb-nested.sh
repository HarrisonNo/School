../utils/pintos -v -k -T 10 --qemu  --filesys-size=2 \
  -p build/tests/userprog/comb-nested -a comb-nested \
  -p build/tests/userprog/child-nested -a child-nested \
  -p build/tests/userprog/child-simple -a child-simple \
  -- -q  -f run comb-nested 