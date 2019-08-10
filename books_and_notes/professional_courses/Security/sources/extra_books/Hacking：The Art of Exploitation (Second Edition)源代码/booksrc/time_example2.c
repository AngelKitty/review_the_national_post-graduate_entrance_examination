#include <stdio.h>
#include <time.h>

void dump_time_struct_bytes(struct tm *time_ptr, int size) {
	int i;
	unsigned char *raw_ptr;

	printf("bytes of struct located at 0x%08x\n", time_ptr);
	raw_ptr = (unsigned char *) time_ptr;
	for(i=0; i < size; i++)
	{
		printf("%02x ", raw_ptr[i]);
		if(i%16 == 15) // print a newline every 16 bytes
			printf("\n");
	}
	printf("\n");
}

int main() {
	long int seconds_since_epoch;
	struct tm current_time, *time_ptr;
	int hour, minute, second, i, *int_ptr;

	seconds_since_epoch = time(0); // pass time a null pointer as argument
	printf("time() - seconds since epoch: %ld\n", seconds_since_epoch);

	time_ptr = &current_time;  // set time_ptr to the address of
                              // the current_time struct
	localtime_r(&seconds_since_epoch, time_ptr);

	// three different ways to access struct elements
	hour = current_time.tm_hour;  // direct access
	minute = time_ptr->tm_min;    // access via pointer
	second = *((int *) time_ptr); // hacky pointer access

	printf("Current time is: %02d:%02d:%02d\n", hour, minute, second);

	dump_time_struct_bytes(time_ptr, sizeof(struct tm));

	minute = hour = 0;  // clear out minute and hour
	int_ptr = (int *) time_ptr;

	for(i=0; i < 3; i++) {
		printf("int_ptr @ 0x%08x : %d\n", int_ptr, *int_ptr);
		int_ptr++; // adding 1 to int_ptr adds 4 to the address,
	}             // since an int is 4 bytes in size
}
	
	
