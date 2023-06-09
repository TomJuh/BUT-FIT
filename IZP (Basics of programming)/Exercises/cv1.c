#include <stdio.h>
int my_strchr(char* str,char c);

int main(int argvc, char *argv[]) {
    for (int l=1;l<argvc;l++){
        printf("%d ",my_strchr(argv[l],'o'));
    
    }
    return 0;
}
int my_strchr(char *str,char c){
    char foundCharacter='\0';
    int l=0;
      while (str[l]!='\0'){
            if(str[l]==c){
                foundCharacter = str[l];
                break;
            }
            l++;

    }
 if(foundCharacter=='\0'){
     fprintf(stderr,"Error character not found in: %s\n",str);
     return '\0';
 }
 return l;

}
