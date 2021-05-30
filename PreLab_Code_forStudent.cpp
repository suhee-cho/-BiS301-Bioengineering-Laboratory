
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <fstream>
#include <sstream>

#include <vector>
#include <math.h>

using namespace std;

struct Data
{
    list<unsigned int> sampleList; // sample list
    list<string> clsGeneList; // candidate gene list for classification
};

struct Class
{
    string clsResult; // class of a node - normal or disease; ( inputted only when a node is leaf node, default is null;)
    string clsGene; // gene for best classification
    int cutoffValue; // cutoff value of the clsGene for classification
};


struct Node
{
    Data d;
    Class cls;
    Node* high; // child node (expression => cls_value);
    Node* low; // child node (expression < cls_value);
};

std::string trim(string& str, const string& whitespace = " \t")
{
    unsigned int strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    unsigned int strEnd = str.find_last_not_of(whitespace);
    unsigned int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
};

using string_vector = vector<string>;

string_vector split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while (getline(ss, tok, delimiter)) {
        //Remove whitespaces
        tok = trim(tok);
        internal.push_back(tok);
    }

    return internal;
};

double entropycal(int fOrM, int lAtT) {
    double FoRm = (double)fOrM; double LaTt = (double)lAtT;
    if (fOrM == 0 && lAtT == 0) return 0.00;
    else if (fOrM == 0) return 0.00;
    else if (lAtT == 0) return 0.00;
    else return -(FoRm/(FoRm+LaTt))*log2(FoRm/(FoRm+LaTt)) - (LaTt/(FoRm+LaTt))*log2(LaTt/(FoRm+LaTt)); //[Problem] Fill in the blank. Think how to calcuation of entropy
};


using string_list = list<string>;

class DecisionTree
{
private:
    map<unsigned int, bool> sampleCls; // Sample property: normal(TRUE) or cancer(FALSE)
    map<pair<unsigned int, string>, double> exp; // microarray expression data

public:
    Node dc; // will be used as a root node of a decision tree

    // You may complete following functions at implementation part (part 3);
    void readDataFromFile(string filename); // read & save microarray data into 'exp' variable of this class.
    string_list readDEGFromFile(string filename); // read & return differentially expressed gene list.
    double findCutoffValue(string gene, list<unsigned int> sampleList); // find a best cutoff value of classification for an input gene and input samples.
    double getInfoGain(string gene, double cutoffValue, list<unsigned int> sampleList); // calculate information gain for an input gene and sample list.
    void testReadFile(  );
    void testDEGList (list<string> deg);
    void testInfoGain(string gene, list<double> cutoffValueList, list<unsigned int> sampleList); // Checking information gain value for
    void testCutoff(string gene, list<unsigned int> sampleList); // Checking finding elements from lists
};

void DecisionTree::readDataFromFile(string filename){

    ifstream temp_file;
    string oneline; string onecell;
    temp_file.open(filename);
    if (!temp_file.is_open()) {
        cout << "Error : It can't be opend" << endl;
        exit(0);
    }
    getline(temp_file, oneline);
    istringstream ABC(oneline); getline(ABC, onecell, '\t');
    // onecell = Class
    int cnt = 1;
    while (getline(ABC, onecell, '\t')) {
        // onecell = cancer, cancer, cancer, non-tumor, ...
        if (onecell == "non-tumor") sampleCls.insert({cnt, true}); //[Problem] Fill in the blank. Think how to insert sample class data
        else sampleCls.insert({cnt, false});//[Problem] Fill in the blank. Think how to insert sample class data
        cnt++;
    }
    while (getline(temp_file, oneline)) {
        string Name_p;
        istringstream ABC(oneline); getline(ABC, Name_p, '\t');
        // Name_P = gene id
        int cnt = 1;
        while (getline(ABC, onecell, '\t')) {
            // onecell = exp values
            exp.insert({pair<unsigned int,string>(cnt,Name_p),stod(onecell)}); //[Problem] Fill in the blank. Think how to insert expression data
            cnt++;
        }
    }

    testReadFile();

};

void DecisionTree::testReadFile(  ){
    int cnt;
    // Test 1
    // This is for testing your code.
    // Printing out index and class of it for each sample from `sampleCls` list
    cnt = 0;
    map<unsigned int, bool>::iterator it = sampleCls.begin();
    cout << "TEST1: print list variable" << endl;
    for(it = sampleCls.begin(); it!=sampleCls.end(); it++){
        cout << "Sample " << (*it).first << ": " << ((*it).second ? "Normal" : "Disease")  << endl;
        if (++cnt >= 20){
            cout << "..." << endl;
            break;
        }
    }
    cout << endl;

    // Test2
    // This is for testing your code.
    // Printing out expression value of cetain sample and gene	from exp list
    cnt = 0;
    map<pair<unsigned int, string>, double>::iterator it2 = exp.begin();
    cout << "TEST2: print map variable" << endl;
    for(it2 = exp.begin(); it2!=exp.end(); it2++){
        cout << "(Sample " << (*it2).first.first << ", " << (*it2).first.second << "): " << (*it2).second  << endl;
        if (++cnt >= 20){
            cout << "..." << endl;
            break;
        }
    }
    cout << endl;
};

string_list DecisionTree::readDEGFromFile (string filename)
{
    list<string> deg;
    ifstream temp_file;
    string oneline;
    string onecell;
    temp_file.open(filename);
    if (!temp_file.is_open()) {
        cout << "Error : It can't be opend" << endl;
        exit(0);
    }

    int cnt = 0;

    while (getline(temp_file, oneline)) {
        istringstream ABC(oneline);
        getline(ABC, onecell, '\t');
        if ( cnt++ >= 1)
            deg.push_back(trim(onecell));//[Problem] Fill in the blank.Think how to put trimmed deg data. Pay attention to treat whitespace right behind gene name in each line.
    }

    return deg;
};

void DecisionTree::testDEGList (list<string> deg){
    int cnt;
    // Test3
    // This is for testing your code.
    // Printing out all of the name of DEGs
    cnt = 0;
    list<string>::iterator it;
    cout << "TEST3: print DEG list" << endl;
    for(it= deg.begin(); it!=deg.end(); it++){
        cout << *it << " | " ;
        if (++cnt >= 20){
            cout << "..." << endl;
            break;
        }
    }
    cout << endl;
};


double DecisionTree::findCutoffValue(string gene, list<unsigned int> sampleList){
    /*
        //[Problem] Write your own code in here.Please refer "Lab10_PrelabGuideline.pdf"
        //Any of your own idea for finding classifying value is okay (additional score could be given).
        Pay attention to treat when all classes of samples are same.
    */

    list<unsigned int>::iterator it;
    double SuM_norm = 0; double SuM_cancr = 0; int cnt_n = 0; int cnt_c = 0;
    double norm_avr, cancr_avr;
    double norm_std, cancr_std;
    vector<double> norm_value, cancr_value;

    double val;

    for(it= sampleList.begin(); it!=sampleList.end(); it++){
        if(sampleCls.find(*it)->second){ // for normal sample..
            cnt_n ++;
            val = exp.find({*it,gene})->second;
            SuM_norm += val;
            norm_value.push_back(val);
        }
        else {
            cnt_c ++;
            val = exp.find({*it,gene})->second;
            SuM_cancr += val;
            cancr_value.push_back(val);
        }
    }

    /////////////////// method 1: default method /////////////////////
    /*
    if (cnt_n != 0){
        if (cnt_c != 0) return (SuM_norm/cnt_n + SuM_cancr/cnt_c)/2;
        return SuM_norm/(2*cnt_n);
    }
    return SuM_cancr/(2*cnt_c);
    */

    ////////////////// method 2: use std //////////////////
    if (cnt_n != 0) {
        norm_avr = SuM_norm/cnt_n;
        if (cnt_c == 0) return norm_avr;
        cancr_avr = SuM_cancr/cnt_c;
        
        // calculate stds
        val = 0.0;
        for(vector<double>::iterator it = begin(norm_value); it != end(norm_value); ++it)
            val += pow(*it-norm_avr, 2);
        val = val/cnt_n;
        norm_std = sqrt(val);

        val = 0.0;
        for(vector<double>::iterator it = begin(cancr_value); it != end(cancr_value); ++it)
            val += pow(*it-cancr_avr, 2);
        val = val/cnt_c;
        cancr_std = sqrt(val);

        return (norm_std*norm_avr + cancr_std*cancr_avr)/(norm_std + cancr_std);  
    }
    else return SuM_cancr/cnt_c;
};

void DecisionTree::testCutoff(string gene, list<unsigned int> sampleList){
    int cnt = 0;
    // Test4
    // This is for testing your code.
    // Printing out the classes and indeces of all samples in `sampleList`
    cout << "TEST4: print class of samples in 'sampleList'" << endl;
    list<unsigned int>::iterator it;
    for(it= sampleList.begin(); it!=sampleList.end(); it++){
        cout << "sample#: " << *it << "\t";
        cout << "sample# in map: " << sampleCls.find(*it)->first << "\t"; // get sample number
        cout << "sample class in map: " << (sampleCls.find(*it)->second  ? "Normal" : "Disease") << endl; // get corresponding sample class
        if (++cnt >= 20){
            cout << "..." << endl;
            break;
        }
    }
    cout << endl;

    // Test5
    // This is for testing your code.
    // Printing out the expression value of a gene for all samples in `sampleList`
    cnt = 0;
    cout << "TEST5: find expression value for gene of given sample" << endl;
    cout << "input gene: " << gene << endl;
    for(it= sampleList.begin(); it!=sampleList.end(); it++){
        cout << "Expression value of " << gene << " for sample " << *it << ": "; // print gene names
        cout << exp.find(pair<unsigned int, string>(*it, gene))->second << endl; // find value(expression value) for sample number and gene
        if (++cnt >= 20){
            cout << "..." << endl;
            break;
        }
    }
    cout << endl;


    // Test6
    // This is for test your method(function `findCutoffValue`).
    // Print out the cutoff value for a certain gene
    cout << "TEST6: Find cutoff value for a gene of given samples" << endl;
    cout << "input gene: " << gene << endl;
    cout << "cutoffvalue : " << findCutoffValue(gene, sampleList) << endl;
    cout << endl;
};

double DecisionTree::getInfoGain(string gene, double cutoffValue, list<unsigned int> sampleList){

    double entro_1, entro_h, entro_l;
    int Norm = 0; int Cancr = 0; int Norm_h = 0; int Norm_l = 0; int Cancr_h = 0; int Cancr_l = 0;
    // Norm = # of normal samples in list
    // Norm_h = # of normal samples whose exp value is higher than cutoff
    // and so on...
    list<unsigned int>::iterator it;
    for (it = sampleList.begin(); it != sampleList.end(); it++) {
        if (sampleCls.find(*it)->second) {
            Norm++;
            if (exp.find(pair<unsigned int, string>(*it, gene))->second >= cutoffValue) Norm_h++;
            else Norm_l++;
        }
        else if (!(sampleCls.find(*it)->second)){
            Cancr++;
            if (exp.find(pair<unsigned int, string>(*it, gene))->second >= cutoffValue) Cancr_h++;
            else Cancr_l++;
        }
    }
    /*
    cout << "Norm: " << Norm << endl;
    cout << "Cancr: " << Cancr << endl;
    cout << "Norm_h: " << Norm_h << endl;
    cout << "Cancr_h: " << Cancr_h << endl;
    entro_1 = entropycal(Norm, Cancr);
    entro_h = entropycal(Norm_h, Cancr_h);
    entro_l = entropycal(Norm_l, Cancr_l);
    cout << "entro_1: " << entro_1 << endl;
    cout << "entro_h: " << entro_h << endl;
    cout << "entro_l: " << entro_l << endl;
*/
    double Output = entro_1 - entro_h*(Norm_h+Cancr_h)/(double)(Norm+Cancr) - entro_l*(Norm_l+Cancr_l)/(double)(Norm+Cancr); //[Problem] Fill in the blank. Think how to calculate information gain

    return Output;
};

void DecisionTree::testInfoGain(string gene, list<double> cutoffValueList, list<unsigned int> sampleList){
    // Test6
    // This is for testing your method(function `getInfoGain`).
    // Printing out the InfoGain value for expression value of a gene for all samples in `sampleList`
    list<double>::iterator it;
    cout << "TEST7: Check information gain value for a gene of given samples with various cutoff value" << endl;
    cout << "input gene: " << gene << endl;
    for(it= cutoffValueList.begin(); it!=cutoffValueList.end(); it++){
        cout << "InfoGain of " << gene << " for samples with cutoff " << *it <<" : "; // print gene name
        cout << getInfoGain(gene, *it, sampleList) << endl; // find value(expression value) for sample number and gene
    }
    cout << endl;
};

int main(int argc, char const *argv[]) {
  DecisionTree d;
  d.readDataFromFile("Lab10_GSE13164_train.txt"); // change to proper filename for your case
  string_list deg = d.readDEGFromFile("Lab10_GSE13164_DEG.txt"); // change to proper filename for your case
  d.testDEGList(deg);

  list<unsigned int> l;

  l.push_back(1);
  l.push_back(2);
  l.push_back(4);
  l.push_back(5);
  l.push_back(6);
  l.push_back(3);

 /*
  l.push_back(1);
  l.push_back(2);
  l.push_back(4);
  l.push_back(35);
  l.push_back(40);
  l.push_back(37);
*/
  string_list::iterator it = deg.begin();
  double tmp_cov = d.findCutoffValue(*it,l);
  d.testCutoff(*it, l);
  cout << tmp_cov << endl;

  list<double> cov_l;
  cov_l.push_back(tmp_cov);
  cov_l.push_back(3.1);
  cov_l.push_back(4.4);
  d.testInfoGain(*it, cov_l, l);

  return 0;
}
