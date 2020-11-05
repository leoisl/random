set -eux

# getting data
<removed>

# compiling
g++ -O3 test_read_vcf.cpp -o test_read_vcf_Cpp -Wall
gcc -O3 test_read_vcf.c -o test_read_vcf_C -Wall

# running and profiling CPP code on the three VCFs
/usr/bin/time ./test_read_vcf_Cpp 1.vcf > 1.out.cpp.txt 2>>time.logging.cpp.txt
/usr/bin/time ./test_read_vcf_Cpp 2.vcf > 2.out.cpp.txt 2>>time.logging.cpp.txt
/usr/bin/time ./test_read_vcf_Cpp 3.vcf > 3.out.cpp.txt 2>>time.logging.cpp.txt

# running and profiling C code on the three VCFs
/usr/bin/time ./test_read_vcf_C 1.vcf > 1.out.c.txt 2>>time.logging.c.txt
/usr/bin/time ./test_read_vcf_C 2.vcf > 2.out.c.txt 2>>time.logging.c.txt
/usr/bin/time ./test_read_vcf_C 3.vcf > 3.out.c.txt 2>>time.logging.c.txt

# ensuring all outputs are the same
diff -qs 1.out.cpp.txt 1.out.c.txt
diff -qs 2.out.cpp.txt 2.out.c.txt
diff -qs 3.out.cpp.txt 3.out.c.txt
