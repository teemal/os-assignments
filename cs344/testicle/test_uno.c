#include <stdio.h>
int main(){
FILE *f;
char c;
f = popen("ls -la /../","r");
while((c = fgetc(f)) != EOF){
putchar(c);
}
pclose(f);
return 0; 
}
