/*
	Assignment 2 - Huffman and LZ Coding Schemes.
	
	Name - Anand Raj
	Roll - 17EC10003

	1) Generated The Random Sequence Using TPM;
	2) Used LZ78 Algorithm To Encode The String.
	3) Used Huffman Algorithm To Encode The String.

	Bit Per Symbol In Huffman 	: Around 2.83 (May vary from run to run)
	Bit Per Symbol In LZ78 		: Around 3.33 (May vary from run to run)

	NOTE: 	A character is taken as 8 bit
			while the encoded data is binary (1 bit per symbol).
			Since it is still made as a string in C++
			actual file size may be larger. 
*/


#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
vector<vector<double>> TPM(8,vector<double> (8));
const int MAXLEN=1e6,Codelen=18;
string seq;
vector<string> HuffCode(MAXLEN);
vector<string> Initial = {"000","001","010","011","100","101","110","111"};

//Generate Random Sequence Using The TPM.
string GenSeq()		
{
	string s;
	int last=rng()%8;
	s+=('a'+last);

	while(s.length()<MAXLEN)
	{
		double prob=(rng()%100)/100.0;
		double prefsum=0;
		for(int i=0;i<8;i++)
		{
			if(prob>prefsum&&prob<=prefsum+TPM[last][i])
			{
				s+='a'+i;
				last=i;
				break;
			}
			prefsum+=TPM[last][i];
		}
	}
	return s;

}

//Encoding A Particular Symbol Based On The Binary Tree.
void Encode(int idx,vector<pair<int,int>> &BT,int curr,string upto)
{
	if(BT[curr].first==-1)
	{
		if(curr==seq[idx]-'a') HuffCode[idx]=upto;
	}
	else
	{
		Encode(idx,BT,BT[curr].first,upto+'0');
		Encode(idx,BT,BT[curr].second,upto+'1');
	}
}

//Creates Binary Tree And Encode The Symbol
void EncodeHuffman(int idx)
{
	int last=seq[idx-1]-'a';

	set<pair<double,int>> PS;
	for(int i=0;i<8;i++)
	{
		PS.insert({TPM[last][i],i});
	}

	int curr=8;
	vector<pair<int,int>> BT(16,{-1,-1});	//Binary Tree Stores Index To Left And Right
	while(PS.size()>1)
	{
		pair<double,int> P1,P2;		//Minimum Two Probabilities
		auto it=PS.begin();
		P1=*it;
		it++;
		P2=*it;
		pair<double,int> P3;
		P3.first=P1.first+P2.first;
		P3.second=curr;
		BT[curr++]={P1.second,P2.second};

		PS.erase(P1);		//Erase The Two Minimum and Add The Sum
		PS.erase(P2);
		PS.insert(P3);
	}

	Encode(idx,BT,curr-1,"");
}

int main()
{
	//Initialize Transition Probability Matrix.
	for(int i=0;i<8;i++)
	{
		double sum=0;
		for(int j=0;j<8;j++)
		{
			TPM[i][j]=(rand()%100)/100.0;
			sum+=TPM[i][j];
		}
		for(int j=0;j<8;j++) TPM[i][j]/=sum;
	}

	//Generate Random Sequence
	seq=GenSeq();
	ofstream fout;
	fout.open("A.txt");
	for(auto c:seq)
	{
		fout.put(c);
	}
	fout.close();

	//LZ78 Coding Scheme

	//Initialize The Dictionary
	map<string,int> Dict;
	map<string,string> Code;
	int i=0;
	Dict[""]=0;
	int idx=1;
	string s="";

	//Populate The Dictionary
	while(i<MAXLEN)
	{
		if(Dict.find(s+seq[i])==Dict.end())
		{
			Dict[s+seq[i]]=idx;
			s="";
			idx++;
		}
		else
		{
			s+=seq[i];
		}
		i++;
	}
	
	//Coding For The Dictionary Items
	for(auto words:Dict)
	{
		if(words.first=="") continue;
		char x=words.first.back();
		string temp=words.first;
		temp.pop_back();
		string code=bitset<Codelen>(Dict[temp]).to_string();
		code+=Initial[x-'a'];
		Code[words.first]=code;
	}

	//Encoding The LZ Code
	string LZCode;
	i=0;
	string temp="";
	while(i<MAXLEN)
	{
		if(Code.find(temp+seq[i])==Code.end())
		{
			LZCode+=Code[temp];
			temp="";
		}
		else
		{
			temp+=seq[i];
			i++;
		}
	}
	if(temp!="") LZCode+=Code[temp];
	int TotalLZLength = LZCode.length();
	fout.open("B.txt");
	for(auto c:LZCode)
	{
		fout.put(c);
	}
	fout.close();
	
	cout<<"\n+++LZ78 Coding Scheme+++\n"<<endl;
	cout<<"Sequence Length (Symbols): "<<MAXLEN<<endl;
	cout<<"Sequence Size (Bits): "<<8*MAXLEN<<endl;
	cout<<"LZ Code Size (Bits): "<<TotalLZLength<<endl;
	cout<<"Compression Ratio: "<<TotalLZLength/(8.0*MAXLEN)<<endl;
	cout<<"Bits Per Symbol: "<<TotalLZLength*1.0/MAXLEN<<endl;


	//Huffman Coding
	HuffCode[0]=Initial[seq[0]-'a'];
	for(int idx=1;idx<MAXLEN;idx++)
	{
		EncodeHuffman(idx);
	}
	fout.open("C.txt");
	int TotalHuffmanLength=0;
	for(int i=0;i<MAXLEN;i++)
	{
		for(auto c:HuffCode[i])
		{
			TotalHuffmanLength++;
			fout.put(c);
		}
	}
	fout.close();
	cout<<"\n+++Huffman Coding Scheme+++\n"<<endl;
	cout<<"Sequence Length (Symbols): "<<MAXLEN<<endl;
	cout<<"Sequence Size (Bits): "<<8*MAXLEN<<endl;
	cout<<"Huffman Code Size (Bits): "<<TotalHuffmanLength<<endl;
	cout<<"Compression Ratio: "<<TotalHuffmanLength/(8.0*MAXLEN)<<endl;
	cout<<"Bits Per Symbol: "<<TotalHuffmanLength*1.0/MAXLEN<<endl;

}