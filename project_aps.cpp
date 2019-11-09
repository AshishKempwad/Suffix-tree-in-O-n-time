#include<bits/stdc++.h>
using namespace std;


typedef struct Suffix st_node;
st_node *root;
st_node *lastnew_node = NULL; 
st_node *active_node = NULL; 

int remaining = 0; 
// active points
int active_edge = -1; 
int active_length = 0; 
int string_size = -1; 

//strcture of suffix
struct Suffix 
{ 
    struct Suffix *nodes[256]; 
    struct Suffix *suffix_link; 
    int start; 
    int *end; 
    int suffix_index; 
}; 

int final_end = -1;

string input; 


int find_length(st_node *n)
{ 
    return *(n->end) - (n->start) + 1; 
}

// constructor
st_node *new_node(int start, int *end) 
{ 
    st_node *node = new st_node; 
    int i; 
    for (i = 0; i < 256; i++) 
    {
        node->nodes[i] = NULL; 
    }
    node->suffix_link = root; 
    node->start = start; 
    node->end = end; 
    node->suffix_index = -1; 
    return node; 
} 

int edge_length(st_node *n)
{ 
    return find_length(n) ; 
} 



// 3 rules of extension
void rules_of_extension(int index)
{
    //rule 1
    final_end = index;
    remaining++;
    while(remaining > 0) 
    {
        if (active_length == 0)
        {
        active_edge = index; 
        }
        if (!active_node->nodes[input[active_edge]])
        {
            //rule 2
            active_node->nodes[input[active_edge]] = new_node(index, &final_end);
            if (lastnew_node)
            {
                lastnew_node->suffix_link = active_node;
                lastnew_node = NULL;
            }
        }
        else
        {
            //rule 3 (the show stopper)
            st_node *next = active_node->nodes[input[active_edge]];
            if (active_length >= edge_length(next))
            {
                active_edge++; 
                active_length--; 
                active_node=next; 
                continue;
            } 
            if (input[next->start + active_length] == input[index])
            {
                if(lastnew_node && active_node != root)
                {
                    lastnew_node->suffix_link = active_node;
                    lastnew_node = NULL;
                }
            active_length++;
            break;
            }
            int *point_of_split = NULL; 
            point_of_split = new int;
            *point_of_split = next->start + active_length - 1;
            
            st_node *split = new_node(next->start, point_of_split);
            
            active_node->nodes[input[active_edge]] = split;

            split->nodes[input[index]] = new_node(index, &final_end);
            
            next->start += active_length;
            
            split->nodes[input[next->start]] = next;

            if (lastnew_node)
            {
                lastnew_node->suffix_link = split;
            }
            lastnew_node = split;
        }
        remaining--;
        if (active_node == root && active_length > 0) 
        {
            active_length--;
            active_edge ++;
        }
        else if (active_node != root) 
        {
            active_node = active_node->suffix_link;
        }
    }
} 


void dfs(st_node *n, int labelHeight) 
{ 
    if (!n) 
    {
        return;
    } 
    if (n->start != -1)
    { 
        for(int i=n->start;i<=*(n->end);i++)
        {
            cout<<input[i];
        } 
    } 
    int leaf = 1;
    for ( int i= 0; i < 256; i++) 
    { 
        if (n->nodes[i]) 
        { 
            if (leaf == 1 && n->start != -1) 
            {
                cout<< " "<<'['<<n->suffix_index<<']'<<endl; 
            }
            leaf = 0; 
            dfs(n->nodes[i], labelHeight + edge_length(n->nodes[i])); 
        } 
    } 
    if (leaf == 1 && leaf!=0) 
    { 
        n->suffix_index = string_size - labelHeight; 
        cout<<" "<<'['<<n->suffix_index<<']'<<endl; 
    } 
} 

int main() 
{ 
    string t;
      cout<<"Enter the string"<<endl;
    cin>>t;

    input=t+'$';
    string_size = input.size(); 
    int rootEnd = - 1; 

    root = new_node(-1, &rootEnd); 

    active_node = root;
    
    for (int i=0; i<string_size; i++) 
    {
        rules_of_extension(i); 
    }
    dfs(root, 0); 

    return 0; 
} 