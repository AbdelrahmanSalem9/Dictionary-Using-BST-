#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *data;
    struct Node *leftChild;
    struct Node *rightChild;

} Node;

Node* newNode(char *data)
{
    Node *newNode=malloc(sizeof(Node));

    newNode->data=(char*)malloc(strlen(data)+1);
    strcpy(newNode->data,data);

    newNode->leftChild=NULL;
    newNode->rightChild=NULL;

    return newNode;

}

Node* insert(Node *root,char *data)
{

    if(!root)
    {

        Node *n=newNode(data);
        return n;
    }
    else if(strcasecmp(root->data,data)==-1)
    {
        root->leftChild=insert(root->leftChild,data);
    }

    else if(strcasecmp(root->data,data)==1 || strcasecmp(root->data,data)==0)
    {
        root->rightChild= insert(root->rightChild,data);
    }

    return root;
}
int search(Node *root,char *key)
{
    if(!root)
        return 0;

    if(strcasecmp(root->data,key)==0)
        return 1;

    else if(strcasecmp(root->data,key)==1)
        search(root->rightChild,key);

    else if(strcasecmp(root->data,key)==-1)
        search(root->leftChild,key);

}


Node* Load(Node *root,char *fileName)
{
    int size=0;
    FILE *f=fopen(fileName,"r");
    while(!feof(f))
    {
        //assuming not word more than 30 characters
        char word[30];
        fscanf(f,"%[^\n]%*c",&word);
        root=insert(root,word);
        size++;
    }
    fclose(f);
    printf("Dictionary LOADED SUCCESSFULLY......!\n");
    printf(".........................\n");
    printf("size = %d\n",size);
    printf(".........................\n");
    printf("Height = %d\n",findHeight(root));
    printf(".........................\n");
    return root;
}

//Find The Height of the Binary Search Tree
int findHeight(Node *root)
{
    if(root==NULL)
        return -1 ;

    int heightLeft=findHeight(root->leftChild);
    int heightRight=findHeight(root->rightChild);

    return (heightLeft>heightRight ? heightLeft : heightRight) +1;
}

//Find The Minimum value of any tree or subtree
Node* FindMin(Node *tree)
{
    if(!tree)
        return NULL;

    while(tree->leftChild!=NULL)
        tree=tree->leftChild;

    return tree;

}
//Find The Maximum value of any tree or subtree
Node* findMax(Node *tree)
{
    if(!tree)
        return NULL;

    while(tree->rightChild!=NULL)
        tree=tree->rightChild;

    return tree;
}
//getting successor of a specific Node
Node* getSuccessor(Node *root,Node *current)
{
    if(!current ||!root)
        return NULL;
    //have right Subtree

    if(current->rightChild!=NULL)
        return FindMin(current->rightChild);

    //No Right Sub-tree
    else
    {
        Node *successor=NULL;
        Node *ancestor=root;
        while(current!=ancestor)
        {
            if(strcasecmp(ancestor->data,current->data)==-1)
            {
                successor=ancestor;
                ancestor=ancestor->leftChild;
            }
            else if(strcasecmp(ancestor->data,current->data)==1)
                ancestor=ancestor->rightChild;
        }
        return successor;
    }
}

//getting predecessor of a specific Node
Node* getPredecessor(Node *root,Node *current)
{
    if(!root || !current)
        return NULL;

    //have left subtree
    if(current->leftChild!=NULL)
        return findMax(current->leftChild);

    //no Left Subtree

    else
    {
        Node *predecessor=NULL;
        Node *ancestor=root;
        while(ancestor!=current)
        {
            if(strcasecmp(ancestor->data,current->data)==1)
            {
                predecessor=ancestor;
                ancestor=ancestor->rightChild;
            }
            else if(strcasecmp(ancestor->data,current->data)==-1)
                ancestor=ancestor->leftChild;
        }
        return predecessor;
    }
}
//get the leaf of not found word
Node* getSuggestion(Node *root,char *word)
{
    //reach the leaf
    if(strcasecmp(root->data,word)==-1)
    {
        if(root->leftChild!=NULL)
            getSuggestion(root->leftChild,word);
        else
            return root;
    }
    else
    {
        if(root->rightChild!=NULL)
            getSuggestion(root->rightChild,word);
        else
            return root;
    }
}

void check(Node *root,char* sentence)
{
    char *token=strtok(sentence," ");
    while(token!=NULL)
    {
        printf("%s - ",token);
        if(search(root,token)==1)
            printf("Correct");
        else
        {
            printf("Incorrect, ");
            Node *leaf=getSuggestion(root,token);
            printf("Suggestions : %s %s %s",leaf->data,getPredecessor(root,leaf)->data,getSuccessor(root,leaf)->data);
        }
        printf("\n");
        token=strtok(NULL," ");

    }

}

int main()
{
    Node *root=NULL;
    char sentence[100];
    root=Load(root,"EN-US-Dictionary.txt");
    while(1)
    {
        printf("Enter a sentence :");
        gets(sentence);
        check(root,sentence);
        printf("\n");
    }
    return 0;
}
