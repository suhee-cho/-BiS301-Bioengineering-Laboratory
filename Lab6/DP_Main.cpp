#include "Cell.h"
#include "DPmat.h"

int main()
{
	// basic case
	//char seq_x[] = "TGCTCA";
	//char seq_y[] = "TGCTCGTA";

	// multiple result check case
	/*
	char seq_x[] = "TTCCG";
	char seq_y[] = "TAACTCG";

	DPmat SA(seq_x,seq_y);
	SA.fill_in_DPmat();

	SA.print_mat();
	cout<<"y: "<<seq_y<<endl;
    cout<<"x: "<<seq_x<<endl<<endl;

	SA.trace_back(SA.get_LEN_X(), SA.get_LEN_Y(), SA.get_LEN_Y()-1);
	*/
	int i;
	char seq_y[] = "ATGACCGTAATAGGT";
	char* seq_x[] = {"AGTGGTAACT", "TGACAGTACT", "AACCTTGTCT"};
	
	for(i = 0; i<3;i++){
		DPmat SA(seq_x[i],seq_y);
		SA.fill_in_DPmat();
		cout<<"Candidate #"<<i+1<<endl;
		cout<<"x: "<<seq_x[i]<<endl;
		SA.trace_back(SA.get_LEN_X(), SA.get_LEN_Y(), SA.get_LEN_Y()-1);
		cout<<"-----------------------"<<endl;
	}
	
	return 0;
}




