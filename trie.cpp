#include<bits/stdc++.h>
using namespace std;

struct trie{
    bool end;
    int fre;
    struct trie* child[256]; //there can be 256 different characters possible in dictonery
    trie(){
        end=false;
        fre=0;
        for(int i=0;i<256;i++){
            child[i]=NULL;
        }
    }
};

void insert(struct trie* root,string s,int cnt){
    struct trie* cur=root;
    for(int i=0;i<s.size();i++){
        int idx=s[i];
        if(cur->child[idx]==NULL)
            cur->child[idx]=new struct trie();
        cur=cur->child[idx];
    }
    cur->end=true;
    cur->fre=cnt;
}

//splitting string in the basis of comma
// input line:Keyboard,139
// processed: keyboard 139
void tokenize(string const &str, const char delim,vector<string> &out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

int search(trie *root,string s)
{
	trie *cur=root;
	for(int i=0;i<s.size();i++)
	{
		if(cur->child[s[i]]!=NULL)
		{
			cur=cur->child[s[i]];
		}
		else
		return false;
	}
 
	if(cur->end==true){
	    return cur->fre;
    }
	return 0;
}


string serialize(trie* root) {
    string result;
    if(!root)
        return result;
    result += "<";
    for(int i = 0; i < 256; i++){
        if(root->child[i]!=NULL){
            result += string(1, i);
            result += serialize(root->child[i]);
        }
    }
    string f=to_string(root->fre);
    result += f+">";
    return result;
} 

trie* deserialize(string data) {
    if(data.empty())
        return NULL;
    trie* root = new trie();
    trie* current = root;
    char prev='$';
    int val=0;
    stack<trie*> stk;
    for(char c: data){
        if(c >='0' && c<='9'){
            val=val*10+(c-'0');
            continue;
        }
        switch(c){
            case '<':
                val=0;
                stk.push(current);
                prev=c;
                break;
            case '>':
                if(prev=='<'){
                    stk.top()->end=true;
                    stk.top()->fre=val;
                    
                }
                stk.pop();
                prev=c;    
                break;
            default:
                val=0;
                current = new trie();
                stk.top()->child[c] = current;
        }
    }
    cout<<endl;
    return root;
}

void printTrie(struct trie* root,string x){
    if(root->end==true)
    {
        cout<<x<<" ";
        cout<<root->fre<<" "<<endl;
    }
    for(int i=0;i<256;i++){
        if(root->child[i]!=NULL){
            x+=(char)(i);
            printTrie(root->child[i],x);
            x.pop_back();
        }
    }
}

int main()
{
    //constructing trie from given CSV
    struct trie *root=new trie();
    ifstream fin;
    string line;
    // Open an existing file
    fin.open("EnglishDictionary.csv");
    while(!fin.eof()){
        fin>>line;
        vector<string> temp;
        tokenize(line,',',temp);
        insert(root,temp[0],stoi(temp[1]));
    }


    //serializing trie in serialize.txt
    string ser=serialize(root);
    std::ofstream out("serialize.txt");
    out << ser;
    out.close();

    //deserializing trie
    string input_from_file;
    string deserializedText;
    ifstream nameFileout;
    nameFileout.open("serialize.txt");
    while (getline(nameFileout, input_from_file))
    {
        deserializedText+=input_from_file;
    }
    nameFileout.close();
    trie *des=deserialize(deserializedText);


    //taking input from user
    string input;
    cin>>input;
    int res=search(des,input);

    if(res)
        cout<<"YES, "<<res<<endl;
    else
        cout<<"NO"<<endl;

    return 0;
}