#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"elk.h"
// Function that loads JS code from a given file.
static jsval_t require(struct js*js,const char*filename){
	char data[32*1024];
	FILE*fp=fopen(filename,"rb");
	if(fp==NULL)return 0;
	size_t len=fread(data,1,sizeof(data),fp);
	return js_eval(js,data,len);
}
static void log(const char*val){
	if(val!=NULL){
		printf("%s\n",val);
	}else{
		fprintf("%s\n","argument NULL");
	}
}

int main(int argc,char*argv[]){
	if(argc<2){
		fprintf(stderr,"Invalid usage\n");
		exit(1);
	}
	char mem[8192];
	struct js*js=js_create(mem,sizeof(mem));
	clock_t beginning=clock();
	js_set(js,js_glob(js),"require",js_import(js,(uintptr_t)require,"jms"));
	jsval_t console=js_mkobj(js);
	js_set(js,js_glob(js),"console",console);
	js_set(js,console,"log",js_import(js,(uintptr_t)log,"vs"));
	js_set(js,console,"warn",js_import(js,(uintptr_t)log,"vs"));
	js_set(js,console,"error",js_import(js,(uintptr_t)log,"vs"));
	js_set(js,console,"debug",js_import(js,(uintptr_t)log,"vs"));
	for(int i=1;i<argc;i++){
		char*buffer=0;
		long length;
		FILE*f=fopen(argv[i],"rb");
		if(f){
			fseek(f,0,SEEK_END);
			length=ftell(f);
			fseek(f,0,SEEK_SET);
			buffer=malloc(length);
			if(buffer){
				fread(buffer,1,length,f);
			}
			fclose(f);
		}
		if(buffer){
			js_eval(js,buffer,strlen(buffer));
			free(buffer);
		}else{
			fprintf(stderr,"Failed to open file\n");
		}
	}
	return EXIT_SUCCESS;
}
