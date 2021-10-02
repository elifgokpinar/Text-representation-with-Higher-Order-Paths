//Elif Gökpınar 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <dirent.h> 
#include <conio.h> 
#include<locale.h> 
#include <math.h>

struct master{
	char categoryName[30];
	char term[20];
	char term2[20];
	char document[50];
	int txtFrequency;
	int categoryFrequency;
	int numberOfDocument;
	float IDF;
	struct master* masternext;
};

struct category{
	char categoryLink[50];
	char categoryName[30];
	int numberOfTxt;
	struct category *cnext;
};

struct document{
	char documentPath[100];
	struct document *dnext;
};
  
  struct master *top30=NULL;
  struct master *twoTermList=NULL;
  struct master *thirdTermList=NULL;
  
  DIR  *dCategory;
  DIR  *dTxt;
  struct dirent *dir;
  struct dirent *dir2;
 
  FILE *readTxt;

  
int main(){ 
	setlocale(LC_ALL, "Turkish"); 

	char term[20];
 	char categoryName[20];
	
	int categoryfrequency=0;
	int txtFrequency=1;
	int constantFrequency=1;
	float idfnumber;
	
 	int totalDocument;
 	int txtNumber;
 	
	struct master *masterHead = NULL;
	struct master *masterHead2 = NULL;
	struct master *masterHead3 = NULL;
	struct category *categoryHead=NULL;
	struct document  *documentHead=NULL;
	
	//Find category names in dataset folder
	dCategory = opendir("./dataset");
	
  if (dCategory)
  {
    while ((dir = readdir(dCategory)) != NULL)
    {
    	if((strcmp(dir->d_name,".")==0 || strcmp(dir->d_name,"..")==0 || (*dir->d_name) == '.' )){
		}
            else{
            int len = strlen(dir->d_name);
			 if(len > 4 && strcmp(dir->d_name+ len - 4, ".txt")==0){
			 }
			 else{
			 	char categoryPath[30];
			 	char categoryShortName[30];
			 	
			 	strcpy(categoryPath,"./dataset/");
			 	strcat(categoryPath,dir->d_name);
				strcpy(categoryShortName,dir->d_name);
				
				addCategory(&categoryHead,categoryPath,categoryShortName);
    }
	
  }
  		
}
closedir(dCategory);
}
		struct category *search=categoryHead; 
		while (search!=NULL){
			txtNumber=0;
			dTxt = opendir(search->categoryLink);
			if (dTxt){
				while ((dir2 = readdir(dTxt)) != NULL){
				if((strcmp(dir2->d_name,".")==0 || strcmp(dir2->d_name,"..")==0 || (*dir2->d_name) == '.' )){
				}
           	    		else{
				int len = strlen(dir2->d_name);
				//If the file is txt file
			 	if(len > 4 && strcmp(dir2->d_name+ len - 4, ".txt")==0){
			 	totalDocument++;
			 	txtNumber++;
				 //Find txt files names in each category
				char categoryPath[30];
				strcpy(categoryPath,search->categoryLink);
			 	strcat(categoryPath,"/");
			 	strcat(categoryPath,dir2->d_name);
			 	
			 	addDocumentPath(&documentHead,categoryPath);
			 	
				readTxt=fopen(categoryPath,"r");
				 while(!feof(readTxt)){
					fscanf(readTxt,"%s",term);
						add(&masterHead,term,categoryPath,search->categoryName,categoryfrequency,txtFrequency,idfnumber);
						add(&masterHead2,term,categoryPath,search->categoryName,categoryfrequency,txtFrequency,idfnumber);
						add(&masterHead3,term,categoryPath,search->categoryName,constantFrequency);
						
						}
				fclose(readTxt);
				 }
			 	
				}
  			}
  			search->numberOfTxt=txtNumber;
			closedir(dTxt);}
		search=search->cnext;
	
}


//-----------for a part--------------
repetitiveWords(masterHead3);
sortCategoryFrequency(masterHead3);
while(deleteEnd(masterHead3)==1){
}
sortAlphabetforDocument(masterHead3);
firstOrder(masterHead3);
//----------------------------
secondOrder(masterHead3,documentHead);
printSecondorThirdOrder(twoTermList,2);	
//-----------------------------
	
//----------for b and c part ---------
frequencyForCategory(masterHead2);
sortCategoryFrequency(masterHead2);
sortAlphabetforFrequency(masterHead2);
getRidOf(masterHead2);
sortCategoryFrequency(masterHead2);

while(deleteEnd(masterHead2)==1){
}
sortAlphabetforFrequency(masterHead2);	

terminDocumentNumber(masterHead,masterHead2);
printCategory(masterHead2,categoryHead);
idf(top30,categoryHead);	
sortIDF(top30);
printftfIdf(top30,categoryHead);

}
//This function adds the category path like .dataset/econ
//Then I set the txt names in the main function.
void addCategory(struct category **header,char *category,char *categoryShortName){
	
    struct category *termNode = malloc(sizeof(struct category));
    strcpy(termNode->categoryLink,category);
    strcpy(termNode->categoryName,categoryShortName);
    termNode->cnext= NULL;
    
    if(*header == NULL)
        *header = termNode;
    else
    {
        struct category *search = *header;
        while(search->cnext != NULL)
        {
            search = search->cnext;
        }
        search->cnext = termNode;
    }
}
//This function adds the document path in new linked list.It's struct type is document.
void addDocumentPath(struct document **header,char *documentPath){
	
    struct document *termNode = malloc(sizeof(struct document));
    strcpy(termNode->documentPath,documentPath);
    termNode->dnext= NULL;
    
    if(*header == NULL)
	*header = termNode;
    else
    {
        struct document *search = *header;
        while(search->dnext != NULL)
        {
            search = search->dnext;
        }
        search->dnext = termNode;
    }
}

//This function adds the terms of txt documents and categoryNames.
void add(struct master **header,char *oneterm,char* documentPath ,char *categoryName,int frequency,int txtFrequency,float idfnumber){

    struct master *termNode = malloc(sizeof(struct master));
    strcpy(termNode->term,oneterm);
    strcpy(termNode->categoryName,categoryName);
    strcpy(termNode->document,documentPath);
    termNode->txtFrequency=txtFrequency;
    termNode->IDF=idfnumber;
    termNode->categoryFrequency=frequency;
    termNode->masternext= NULL;

    if(*header == NULL)
	*header = termNode;
    else
    {
        struct master *search = *header;		
        while(search->masternext != NULL)
        {
            search = search->masternext;
        }
        search->masternext = termNode;
    }
}
//Prints the first order reliations.
void firstOrder(struct master* list){
    struct master *tempList =list;
    struct master *traverse=list;
   
    printf(" \n*1st-order term co-occurance* \n");
    while(tempList->masternext!=NULL){
    		if(strcmp(tempList->document,tempList->masternext->document)==0){
    		printf("{%s,%s}",tempList->term,tempList->masternext->term);	
		}
	tempList=tempList->masternext;
    }
	
}
//This function finds the repetitive word.
void repetitiveWords(struct master* list){
	struct master* tempList=list;
	struct master* traverse=list;
	
	while(tempList!=NULL){

		traverse=tempList->masternext;
		while(traverse!=NULL){
			if(strcmp(tempList->document,traverse->document)==0){
				if(strcmp(tempList->term,traverse->term)==0){
					tempList->categoryFrequency=0;
				}		
			}						
		traverse=traverse->masternext;
		}
		
		tempList=tempList->masternext;
	}
}
//This function makes a insert for second order.
void secondOrderInsert(struct master** header,char* term,char* term2){
	
    struct master *termNode = malloc(sizeof(struct master));
    strcpy(termNode->term,term);
    strcpy(termNode->term2,term2);
    termNode->masternext= NULL;

    if(*header == NULL)
        *header = termNode;
    else
    {
        struct master *search = *header;
        while(search->masternext != NULL)
        {
            search = search->masternext;
        }
        search->masternext = termNode;
    }
}
//Finds the second order relationship between documents.
void secondOrder(struct master* list,struct document* list2){
	struct master *tempList =list;
	struct master *traverse =list;
	struct document *docList =list2;
	struct document *docTraverse =list2;
	struct master *secondOrderHead=NULL;

	while(docList!=NULL){
		docTraverse=docList->dnext;
		
		while(docTraverse!=NULL){
				tempList=list;
				while(tempList!=NULL){
					if(strcmp(tempList->document,docList->documentPath)==0){
					traverse=list;
						while(traverse!=NULL){
							if(strcmp(traverse->document,docTraverse->documentPath)==0){
								if(strcmp(tempList->term,traverse->term)==0){
								secondOrderInsert(&secondOrderHead,tempList->term,"");
								}
							}
						traverse=traverse->masternext;
						}
					}
				tempList=tempList->masternext;
	}
		if(secondOrderHead!=NULL){
				tempList =list;
				traverse =list;
			
				while(tempList!=NULL){
						if(strcmp(tempList->document,docList->documentPath)==0 && isMatch(secondOrderHead,tempList->term)){
						traverse =list;
							while(traverse!=NULL){
								if(strcmp(traverse->document,docTraverse->documentPath)==0&&isMatch(secondOrderHead,traverse->term)){
								secondOrderInsert(&twoTermList,tempList->term,traverse->term);
								
								}
							traverse=traverse->masternext;	
							}			
							}
							tempList=tempList->masternext;
				}
		}			
		secondOrderHead=NULL;
		docTraverse=docTraverse->dnext;
		}
	docList=docList->dnext;
	}
}
//prints the second order or third order reliationship.
void printSecondorThirdOrder(struct master* list,int number){
	
	struct master* tempList=list;
	struct master* traverse=list;
	
	printf("\n\n* %dnd-order term co-occurance*\n",number);
	while(tempList!=NULL){
		traverse=tempList->masternext;
		while(traverse!=NULL){
			if(strcmp(tempList->term,traverse->term)==0 && strcmp(tempList->term2,traverse->term2)==0){
				strcpy(traverse->term,"");
				strcpy(traverse->term2,"");
			}
			if(strcmp(tempList->term,traverse->term2)==0 && strcmp(tempList->term2,traverse->term)==0){
				strcpy(traverse->term,"");
				strcpy(traverse->term2,"");
			}
			
			traverse=traverse->masternext;
		}
		tempList=tempList->masternext;
	}
	
     tempList=list;
	
	while(tempList!=NULL){
		if(strcmp(tempList->term,"")!=0){
		printf("{%s,%s}",tempList->term,tempList->term2);
		}	
		tempList=tempList->masternext;
	}
}

int deleteSecondOrder(struct master* head){
struct master *temp =head;
  struct master *t;
  if(head->masternext==NULL)
  {
    free(head);
    strcpy(head->term,"");
    head=NULL;
    return 0;
  }
  else
  {
     while(temp->masternext != NULL)
     {
        t=temp;
        temp=temp->masternext;
     }
     free(t->masternext);
     t->masternext=NULL; 
     
     return 1;
  } 

}

int isMatch(struct master* list,char* term){
	struct master *tempList =list;
	
	int x=0;
	
	while(tempList!=NULL){
		if(strcmp(tempList->term,term)==0){
			 x=1;
		}
	tempList=tempList->masternext;
	}
	if(x==1){
		return 0;
	}
	else 
	return 1;
	
}

//Find frequency words in each category;
void frequencyForCategory(struct master* list){
	
	char tempTerm[30];
	struct master* tempList=list;
	struct master* tempListFrequency=list;
	//struct category* tempCategory=list2;
	
	while(tempList!=NULL){
		tempListFrequency=list;
		while(tempListFrequency!=NULL){
		if(strcmp(tempList->term,tempListFrequency->term)==0){
			if(strcmp(tempList->categoryName,tempListFrequency->categoryName)==0){
				tempList->categoryFrequency=tempList->categoryFrequency+1;
			}
		}
		tempListFrequency=tempListFrequency->masternext;
		}
	tempList=tempList->masternext;
	}

}
//Sorts the terms' frequency for each category
void sortCategoryFrequency(struct master **sortList){
    struct master *termNode =sortList;
    struct master *search,*max;
    while(termNode!=NULL){
    	max=termNode;
    	search=termNode->masternext;
    	while(search !=NULL){
    		if(max->categoryFrequency<search->categoryFrequency){
    			max=search;
			}
			search=search->masternext;
		}
		swap(termNode,max);
		termNode=termNode->masternext;
	}
}
//Sorts the terms with their documents.
void sortAlphabetforDocument(struct master* list){
    struct master *termNode =list;
    struct master *search,*presedence;
    while(termNode!=NULL){
    	presedence=termNode;
    	search=termNode->masternext;
    	while(search !=NULL){
    		if(strcmp(presedence->document,search->document)==0){
    			if(strcmp(presedence->term,search->term)==1){
    			presedence=search;	
				}
			}    		
			search=search->masternext;
    		
		}
		swap(termNode,presedence);
		termNode=termNode->masternext;
	}
}
//Swaps the two nodes when they have to change because of sorting.
void swap(struct master* p1,struct master* p2){
	
	int temp = p1->categoryFrequency;
	p1->categoryFrequency = p2->categoryFrequency;
	p2->categoryFrequency = temp;
	
	char tempTerm[30];
	strcpy(tempTerm,p1->term);
	strcpy(p1->term,p2->term);
	strcpy(p2->term,tempTerm);
	
	char tempCategory[30];
	strcpy(tempCategory,p1->categoryName);
	strcpy(p1->categoryName,p2->categoryName);
	strcpy(p2->categoryName,tempCategory);
	
	char tempDocumentPath[30];
	strcpy(tempDocumentPath,p1->document);
	strcpy(p1->document,p2->document);
	strcpy(p2->document,tempDocumentPath);
	
	int temp2 = p1->txtFrequency;
	p1->txtFrequency = p2->txtFrequency;
	p2->txtFrequency = temp2;
	
	float temp3 = p1->IDF;
	p1->IDF = p2->IDF;
	p2->IDF = temp3;
	
}
//After the category frequency is defined,sorts the terms for alphabet.
void sortAlphabetforFrequency(struct master* list){
    struct master *termNode =list;
    struct master *search,*presedence;
    while(termNode!=NULL){
    	presedence=termNode;
    	search=termNode->masternext;
    	while(search !=NULL){
    		if(presedence->categoryFrequency==search->categoryFrequency){
    			if(strcmp(presedence->term,search->term)==1){
    			presedence=search;
			}	
		}    		
			search=search->masternext;
		}
		swap(termNode,presedence);
		termNode=termNode->masternext;
	}
	
}
void  getRidOf(struct master* list){
	struct master* tempList=list;

        struct master *search = tempList;
        struct master *traverse = tempList;
        while(search!= NULL)
        {
        	traverse=search->masternext;
        	while(traverse!=NULL){
        		
        		if(strcmp(search->categoryName,traverse->categoryName)==0){
        			if(strcmp(search->term,traverse->term)==0){
        			traverse->categoryFrequency=0;
        			strcpy(traverse->term,"");
				}
				
				}
				traverse=traverse->masternext;
			}
        search=search->masternext;
		}
    

    }
//Deletes the nodes from end.
int deleteEnd(struct master* list){
	struct master* tempList=list;
	
	while(tempList->masternext->masternext!=NULL){
		tempList=tempList->masternext;
	}
	if(tempList->masternext->categoryFrequency==0){
		free(tempList->masternext);
		tempList->masternext=NULL;
		return 1;
	}
	return 0;
	}
	
void terminDocumentNumber(struct master* list, struct master* list2){
	struct master* masterHead=list;
	struct master* masterHead2=list2;
	
	char issamePath[50];
	strcpy(issamePath,"x");
	
	while(masterHead2!=NULL){
		masterHead=list;
		strcpy(issamePath,"x");
		while(masterHead!=NULL){
			
			if(strcmp(masterHead2->term,masterHead->term)==0){
				if(strcmp(masterHead2->categoryName,masterHead->categoryName)==0){
					if(strcmp(masterHead2->document,masterHead->document)!=0){
						if(strcmp(issamePath,masterHead->document)!=0){
							masterHead2->txtFrequency=masterHead2->txtFrequency+1;
							strcpy(issamePath,masterHead->document);
				}
					
			}	
				}
	
			}
	
		masterHead=masterHead->masternext;
		}
		masterHead2=masterHead2->masternext;
	}
	
}   
//finds the idf*tf value
void idf(struct master* list,struct category* list2){
	struct master *tempList =list;
	struct category* categoryList=list2;
	float N;
	float n;
	float IDF;
		
		while(categoryList!=NULL){
				tempList=list;
				while(tempList!=NULL){
				if(strcmp(categoryList->categoryName,tempList->categoryName)==0){
				N=categoryList->numberOfTxt;
				n=tempList->txtFrequency;
				IDF=log(N/n)*(tempList->categoryFrequency);
				tempList->IDF=IDF;	
				}
				tempList=tempList->masternext;
				}
			categoryList=categoryList->cnext;
		}

	
}
//Sorts the tf*idf values of terms.
void sortIDF(struct master* list){
	struct master *termNode =list;
    struct master *search,*max;
   	int i=10;
    
    while(termNode!=NULL){		
	max=termNode;
    	search=termNode->masternext;
    	while(search !=NULL){
    		
    		if(strcmp(max->categoryName,search->categoryName)==0){
    			if(max->IDF<search->IDF){
    			max=search;
			}	
		}
    		
			search=search->masternext;	

		}
		swap(termNode,max);
		termNode=termNode->masternext;

	}
}
//Prints the part b
void printCategory(struct master* list,struct category* list2){
	struct master* tempList=list;
	struct category* categoryList=list2;
	int i;
	puts("\n");
	while(categoryList!=NULL){
		
		
		printf("\n****Category is %s ****",categoryList->categoryName);
		while(tempList!=NULL){
			if(i<5){
				if(strcmp(categoryList->categoryName,tempList->categoryName)==0){
					add(&top30,tempList->term,tempList->document,tempList->categoryName,tempList->categoryFrequency,
					tempList->txtFrequency,tempList->IDF);
					printf("\n%d) %s ; %d",i+1,tempList->term,tempList->categoryFrequency);
					i++;
				}	
			}
		tempList=tempList->masternext;
		}
		tempList=list;
	categoryList=categoryList->cnext;
	i=0;
	}
}
//Prints the part c
void printftfIdf(struct master* list,struct category*  list2){
	struct master* tempList=list;
	struct category* categoryList=list2;
	int i;
	
	printf("\n---------TF-IDF VALUES---------");
	while(categoryList!=NULL){
		printf("\n****Category is %s ****",categoryList->categoryName);
		tempList=list;
		while(tempList!=NULL){
			
			if(i<5){
				if(strcmp(categoryList->categoryName,tempList->categoryName)==0){
				printf("\n%d) %s ; %.2f",i+1,tempList->term,tempList->IDF);
				i++;
				}	
			}
		tempList=tempList->masternext;
		}
		
	categoryList=categoryList->cnext;
	i=0;
	}
	
}






  
