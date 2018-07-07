#include <iostream>
#include <stdio.h>
#include <fstream>
#include <wchar.h>
#include <sstream>
#include "Ngram.h"
#include "Vocab.h"
#include <map>
#include <stack>

class Proba{
	public:
		char* word;
		VocabIndex wid;
		double prob;
		short jumpfrom;
		bool used;
		Proba(){
			prob = -1e50;
			word = new char[2];
			used = 0;
		}

};

using namespace std;
int main(int argc,char** args){
//	cout<<100<<endl;
	int ngram_order = 2;
	Vocab voc;
	Ngram lm(voc,ngram_order);
	{
		const char lm_inFile[] = "bigram.lm";
		File fil(lm_inFile,"r");
		lm.read(fil);
		
	}

	ifstream ifs;
	ifstream ofs1;
//	ofs1.open("test.txt");	
	string str1 ;
//	ofs1 >>str1;
	stringstream ss1(str1);
	char i1[2];
	ss1>>i1; 
//	cout<<i1[0]<<i1[1]<<endl; 
	string str = "";
	char* s1 = new char[2];
	ifs.open("Zhu-Yin_Big5.map");
//	cout<<args[4]<<endl;
	multimap<short,char*> multi_map;
	if(ifs.is_open()){
		
		while(!ifs.eof()){
	//		cout<<1111<<endl;
			getline(ifs,str);
			stringstream ss(str);
			ss>>s1;
		//	cout<<s1;
	//		cout<<short(short(s1[1]));
			short sh = ((short(s1[0]<<8)&0xff00)|(0x00ff&short(s1[1])));
			while(1){
				char * s = new char[2];
				ss>>s;
		  	//	cout<<s<<"\t";

				multi_map.insert(pair<short,char*>(sh,s));		
				if(ss.eof()){
					break;
				}
				
			}
			;
			
	//		 cout<<endl;
			
			
		}
	}else{
		cout<<"cannot open map\n";
		
	}
	char* sh1 = multi_map.lower_bound((short(i1[0])<<8&0xff00)|(0x00ff&short(i1[1])))->second;
//	cout<<sh1<<"333";
	ifs.close();
//	ofs1.close();
	
	ifs.open(args[1]);
	ofstream ofs;
	ofs.open(args[2]);
//	cout<<args[1]<<args[2]<<args[10]<<endl;

	if(ifs.is_open()){
		str = "";
		while(1){

			getline(ifs,str);
			if(ifs.eof()){
				break;
			}
			stringstream ss(str);
			char* word = new char[2];

			VocabIndex context[2];
			context[0] = voc.getIndex(Vocab_SentStart); //init is "<s>"
			context[1] = Vocab_None;

			Proba veterbi_table[2000][100];
			veterbi_table[0][0].prob = 1;
			veterbi_table[0][0].used = 1;
			veterbi_table[0][0].word = "</s>";
			//veterbi_table[0][0] is "<s>"
			int index = 1;	//record the current column

			/*viterbi_table initialized*/
			int final_max_index = -1;
			while (!ss.eof()){
				ss>>word;
				short short_word;
				short_word = short(word[0]<<8 &0xff00) | short(word[1]&0x00ff);
				/*get a single char[2]*/

				
				
				pair<multimap<short,char*>::iterator,multimap<short,char*>::iterator>ret;
			

				ret = multi_map.equal_range(short_word);

				/*get the candidate for each char[]*/

				int row_index = 0;

				for(multimap<short,char*>::iterator it = ret.first; it!=ret.second; ++it){
					VocabIndex wid = voc.getIndex(it -> second);
					veterbi_table[row_index][index].word 	= it -> second;
					veterbi_table[row_index][index].used 	= 1;
					veterbi_table[row_index][index].wid 	= wid;
			

					double max_probability = -2e50;
					int max_from = -1;
					for(int i = 0;i<2000;i++){
						double probability = 0;
						context[0] = veterbi_table[i][index-1].wid;
						if(context[0] == Vocab_None){
							context[0] = voc.getIndex(Vocab_Unknown);
						
							
						}
						if(wid == Vocab_None){
							wid = voc.getIndex(Vocab_Unknown);
						}
						probability = lm.wordProb(wid,context) + veterbi_table[i][index-1].prob;
					
						if(probability>max_probability){
							max_probability = probability;
							max_from = i;
						}
						if(!veterbi_table[i+1][index -1].used){
							break;
						}
					}

					veterbi_table[row_index][index].jumpfrom 	= max_from;
					veterbi_table[row_index][index].prob	= max_probability;

				
					row_index ++;
				
				}
		// 		row_index = 0;
				
				double final_max_probability = -2e50;
				for(int i = 0; i < 2000 ; i ++){
				//	cout<<i<<endl;
					
					if(!veterbi_table[i][index].used){
				
						break;
					}
				//	cout<<i<<endl;
				//	cout<<final_max_probability<<endl;
					if(veterbi_table[i][index].prob > final_max_probability){
						final_max_probability = veterbi_table[i][index].prob;
						final_max_index = i;

					}
				}
			//	cout<<veterbi_table[final_max_index][index].word[0]<<veterbi_table[final_max_index][index].word[1];
			//	cout<<endl;
				index ++;
			}
			stack<char*> wordArray;
			int prev_jump_from = final_max_index;
			for(int j = index - 1; j >= 1; j--){
				wordArray.push(veterbi_table[prev_jump_from][j].word);
				prev_jump_from = veterbi_table[prev_jump_from][j].jumpfrom;

			}
			ofs<< "<s> ";
			for(int j = 0;j < index - 2; j ++){
				char* ch_chi = wordArray.top();
				ofs <<	ch_chi << " ";
				wordArray.pop();
			}
			ofs<<"<\\s>"<<endl;
		//	cout<<endl;

		}

	}else{

		cout<<"cannot open "<<args[2]<<endl;
		
	}
	ofs.close();


}