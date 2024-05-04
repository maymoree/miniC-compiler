#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



int main(int argc, char* argv[]){
		#if YYDEBUG
			 yydebug = 1;
		#endif

		if (argc == 3){
			yyin = fopen(argv[1], "r");
			if (yyin == NULL) {
				fprintf(stderr, "File open error\n");
				return 1;
			}
		}

		if (semantic_analysis(root)){
			printf("Passed semantic analysis.\n");
		} else {
			printf("Failed semantic analysis.\n");
		}

		
		if (argc == 2) fclose(yyin);
		return 0;
}