#include<stdio.h>
/*
*big endian :high bytes are storge in the little address
*little endian :opposite to big endian
*/
void byteorder()
{
	union{
		short value;
		char union_bytes[sizeof(short)];
	} test_value;
	test_value.value = 0x0102;
	if ((test_value.union_bytes[0] == 1) && (test_value.union_bytes[1] == 2)) {
		printf("big endian\n");
	} else if ((test_value.union_bytes[0] == 2) && (test_value.union_bytes[1] == 1)) {
		printf("little endian\n");
	} else {
		printf("unknown...\n");
	}
}

int 
main(void)
{
	byteorder();
	return 0;
}
