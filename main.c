#include <string.h>
#include "partitions/partition.h"

void show_parts(){
	part parts[50];
	uint64_t size = get_parts(parts, 50);
	printf(" MAJ | MIN | Sizes(in Mb) | Names\n");
	printf(" --- | --- | ------------ | -----\n");
	for (int i = 0; i < size; ++i) {
	    printf(" %3d | %3d | %12lu | %s\n", parts[i].maj, parts[i].min, parts[i].size_mb, parts[i].name);
	}
}

int main(int argc, char **argv) {
	if (strcmp(argv[1], "--parts") == 0) {
		show_parts();
	} else {
		return 0;
	}

}
