#include <iostream>
#include <map>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <time.h>
#include <algorithm>

#include <stdlib.h> 

using namespace std;

struct Data
{
    list<unsigned int> sampleList; // sample list
    list<string> clsGeneList; // candidate gene list for classification
};


struct Class
{
    unsigned int clsResult; // classification (only inputted when the node is leaf node - 0: not determined, 1: normal, 2:disease)
    string clsGene; // gene of best classification (previously defined as int (wrong))
    double cutoffValue; // cutoff value of the clsGene for classification
};

struct Node
{
    Data data; // microarray data (variable name was changed from "d")
    Class cls; // classification information
    Node* high; // son node (expression => cls_value);
    Node* low; // son node (expression < cls_value);
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

string readLine(ifstream &DataName) //read the line of the data
{
    string strLine_local;
    getline(DataName, strLine_local);
    return strLine_local;
}

using string_list = list<string>;

class DecisionTree
{
private:
    void drawGraph(Node* n, string pre); // private function for recursive drawing for drawGraph(Node* n) function.

public:
    Node dc; // decision tree
    map<unsigned int, bool> sampleCls; // Sample property: normal(true) vs. cancer(false)
    map<pair<unsigned int, string>, double> exp; // microarray expression data

    /* Source-provided Functions */
    void drawGraph(Node* n);

    /* Functions for Testing */
    void testReadFile( ); // Checking for inserting MA data file
    void testDEGList(list<string> deg); // Checking for insering DEG list file
    void testCutoff(string gene, list<unsigned int> sampleList); // Checking finding elements from lists
    void testInfoGain(string gene, list<double> cutoffValueList, list<unsigned int> sampleList); // Checking information gain value for some samples

    /* Prelab Work Below*/
    void readDataFromFile(string filename); // read & save microarray data into 'exp' variable of this class.
    string_list readDEGFromFile(string filename); // read & return differentially expressed gene list.
    double findCutoffValue(string gene, list<unsigned int> sampleList); // find a best cutoff value of classification for an input gene and input samples.
    double getInfoGain(string gene, double cutoffValue, list<unsigned int> sampleList); //calculate information(entropy) for an input gene and sample list.

    /* Functions You should implement in Lab */
    void makeDecisionTree(Node* n); // generate Decision tree (recursively executed!)
    bool predSample(unsigned int sampleNo, Node* n); // Test the normal/disease-known sample to the generated Decision tree. -> follow the branching criteria to get final classification and compare with original sample class.
    double getAccuracy(list<unsigned int> sampleList, Node* n); // For given sample list, calculate the average and return the accuracy.
    double nFoldCV(unsigned int n, Data d); // validate with N-fold cross-validation and returns the accuracy


};

void DecisionTree::drawGraph(Node* n){
    drawGraph(n, "");
};

void DecisionTree::drawGraph(Node* n, string pre){
    stringstream ss;
    ss<<(n->cls.cutoffValue);

    if(n->cls.clsResult!=0){
        cout << " -[" << (n->cls.clsResult==1 ? "Normal" : "Tumor") << "]" << endl;
    }else{
        cout << "* " + n->cls.clsGene + "(" + ss.str() + ")" << endl;
        cout << pre << "|- >";
        drawGraph(n->high, pre+"| ");
        cout << pre << "|- <";
        drawGraph(n->low, pre+"    ");
    }
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
    entro_1 = entropycal(Norm, Cancr);
    entro_h = entropycal(Norm_h, Cancr_h);
    entro_l = entropycal(Norm_l, Cancr_l);

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

void DecisionTree::readDataFromFile(string filename){

    ifstream temp_file;
    string oneline; string onecell;
    temp_file.open( filename);
    if (!temp_file.is_open()) {
        cout << "Error : It can't be opend" << endl;
        exit(0);
    }
    getline(temp_file, oneline);
    istringstream ABC(oneline); getline(ABC, onecell, '\t');
    int cnt = 1;
    while (getline(ABC, onecell, '\t')) {
        if (onecell == "non-tumor") sampleCls.insert({cnt, true});//[Problem] Copy and paste from your prelab code. Fill in the blank. Think how to insert sample class data
        else sampleCls.insert({cnt, false});//[Problem] Copy and paste from your prelab code. Fill in the blank. Think how to insert sample class data
        cnt++;
    }
    while (getline(temp_file, oneline)) {
        string Name_p;
        istringstream ABC(oneline); getline(ABC, Name_p, '\t');
        int cnt = 1;
        while (getline(ABC, onecell, '\t')) {
            exp.insert({pair<unsigned int,string>(cnt,Name_p),stod(onecell)}); //[Problem] Copy and paste from your prelab code. Fill in the blank. Think how to insert expression data
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
            deg.push_back(trim(onecell));//[Problem] Copy and paste from your prelab code. Fill in the blank.Think how to put trimmed deg data. Pay attention to treat whitespace right behind gene name in each line.
    }

    return deg;
};

void DecisionTree::makeDecisionTree(Node* n){
    // Your Code 
    list<unsigned int>::iterator S_iterator;
    int countNum_Normal = 0; //number of normal sample
    int countNum_Total = 0; //number of total sample
    int countNum_Disease = 0; //number of disease sample
    for (S_iterator = n->data.sampleList.begin();
        S_iterator != n->data.sampleList.end(); S_iterator++) {
        countNum_Total++; //counting the total number of the sample
        if (sampleCls.find(*S_iterator)->second) {
            countNum_Normal++; //counting the total number of normal sample
        } else {
            countNum_Disease++; //counting the total number of disease sample
        }
    }
    //cout << "p1" << endl;
    if (countNum_Total == countNum_Normal) { //if all the samples are normal
        n->cls.clsResult = 1; //change the classfication status to 'normal'
        //cout << "leaf reached." << endl;
        return; //and now terminate
    } else if (countNum_Total == countNum_Disease) { //if all the samples are disease samples
        n->cls.clsResult = 2; //change the classification status to 'disease'
        //cout << "leaf reached." << endl;
        return; //and now finish the loop
    }
// if (sizeof(n->data.clsGeneList) == 0)
    if (n->data.clsGeneList.empty()) { //if there is no more gene to be used
        if (countNum_Normal > countNum_Total - countNum_Normal) { //and if the number of normal samples are more than the disease samples
            n->cls.clsResult = 1; //change the classification status to 'normal'
        } else { //if not,
            n->cls.clsResult = 2; //change the classification status to 'disease'
        }
        //cout << "leaf reached." << endl;
        return;
    } else {
        n->cls.clsResult = 0; //if the case above all does not fit, this node shall go more. Set the classification status to 'NULL'

        list<string>::iterator D_iterator;
        string Optimal_Gene; 
        double max_Gene = 0; //maximum value of information gain of the gene
        // double current_Gene = 0; // unused
        for (D_iterator = n->data.clsGeneList.begin();
            D_iterator !=
            n->data.clsGeneList.end(); D_iterator++) { //using the iterator, read all the genes in DEG list of the current node
            double temp_COV = findCutoffValue(*D_iterator, n->data.sampleList); //[MainLabProblem] find the cut off value for the gene
            double current_Gene = getInfoGain(*D_iterator, temp_COV, n->data.sampleList); //[MainLabProblem] find the information gain for the gene
            if (max_Gene <= current_Gene) {
                max_Gene = current_Gene;
                Optimal_Gene = *D_iterator; //[MainLabProblem] find the gene with maximum information gain
            }
        }
        n->cls.clsGene = Optimal_Gene; //[MainLabProblem] the Optimal_Gene, which has the maximum information gain, will be the classification gene
        n->cls.cutoffValue = max_Gene; //[MainLabProblem] find the cut off value for the gene

        list<unsigned int> lowstring; //this will save all the low expressed samples
        list<unsigned int> highstring; //this will save all the high expressed samples
        list<string> new_DEG;
        new_DEG = n->data.clsGeneList;
        new_DEG.remove(Optimal_Gene); // remove the gene that is used for classification
        for (S_iterator = n->data.sampleList.begin();
             S_iterator != n->data.sampleList.end(); S_iterator++) {
            if (exp.find({*S_iterator,Optimal_Gene})->second > max_Gene) { //[MainLabProblem] if the expression level of the sample is higher than the cut off value,
                highstring.push_back(*S_iterator); //put the high expressed samples in the highstring
            } else { //if not,
                lowstring.push_back(*S_iterator); //put the low expressed samples in the lowstirng
            }
        }
        n->high = new Node;
        n->low = new Node;
        n->high->data.sampleList = highstring; //highly expressed samples are sent to the high node
        n->high->data.clsGeneList = new_DEG; //the gene list which elminated the used gene is sent to the high node
        n->low->data.sampleList = lowstring; //low expressed samples are sent to the low node
        n->low->data.clsGeneList = new_DEG; //the gene list which elminated the used gene is sent to the low node
        DecisionTree::makeDecisionTree(n->low); //[MainLabProblem] do the same in the low node. (recursively)
        DecisionTree::makeDecisionTree(n->high); //[MainLabProblem] do the same in the high node. (recursively)
    }
};

bool DecisionTree::predSample(unsigned int sampleNo, Node* n){

    // Your Code
    bool torf;
    if (n->cls.clsResult != 0) { //if the current node is a leaf node
        if (n->cls.clsResult == 1) { //and if the current node says normal,
            torf = true; //[MainLabProblem] this sample is normal
        } else if (n->cls.clsResult == 2) { //else if the current node says disease,
            torf = false; //[MainLabProblem] this sample is disease
        }
    } else if (n->cls.clsResult == 0) { //else if the current node is not a leaf node,
        if (n->cls.cutoffValue
            > exp.find(pair<unsigned int, string>(sampleNo, n->cls.clsGene))->second) { //and if the sample's expression level is lower than the cut off value
            torf = predSample(sampleNo, n->low); //[MainLabProblem] go to the low node
        } else { //if not
            torf = predSample(sampleNo, n->high); //[MainLabProblem] go to the high node
        }
    }
    return torf; //return whether the sample is disease or not
};

double DecisionTree::getAccuracy(list<unsigned int> sampleList, Node* n){
    // For the given sample list, test each samples and calculate accuracy.
    double truepositive = 0;
    double falsepositive = 0;
    double truenegative = 0;
    double falsenegative = 0;
    bool trueorfalse;
    bool real_diagnosis;
    list<unsigned int>::iterator S_iterator;
    for (S_iterator = sampleList.begin(); S_iterator != sampleList.end();
         S_iterator++) { //read all the samples
        /*
            //[MainLabProblem] Write your own code in here.
            count truepositive, falsepositive, truenegative and falsenegative
        */
        if (sampleCls.find(*S_iterator)->second^predSample(*S_iterator, n)){
            if (predSample(*S_iterator, n)) falsepositive ++;
            else falsenegative ++;
        }
        else {
            if (predSample(*S_iterator, n)) truepositive ++;
            else truenegative ++;
        }
    }
    double accuracy = (truepositive+truenegative)/(truepositive+truenegative+falsepositive+falsenegative); //[MainLabProblem] calculate accuracy
    return accuracy; //return the accuracy
};

double DecisionTree::nFoldCV(unsigned int n, Data d){
    unsigned int Num_Total = 0;
    double Accuracy_Avg = 0;
    map<unsigned int, bool> sample_map; //this map will determine us whether the samples are already used to make a subset or not
    list<unsigned int>::iterator S_iterator;
    for (S_iterator = d.sampleList.begin(); S_iterator != d.sampleList.end();
         S_iterator++) { //for all samples
        sample_map.insert(pair<unsigned int, bool>(*S_iterator, false)); //inset the sample number, and the value 'false' since we didn't use any of them yet.
        Num_Total++; //count the total number of samples
    }
    map<unsigned int, list<unsigned int> > sublist; //this will save all the sublist that is made in this function
    unsigned int randNum;
    unsigned int countNum = 0; //count the number of samples that are used to make a sublist
    unsigned int SublistNum = 0; //this will determine where to put the sample numbers
    srand(time(NULL)); //change the random pattern
    while (countNum != Num_Total) { //if the samples are not all used to make a sublist
        randNum = rand() % (Num_Total) + 1; //make any random number between 1~maximum number of the sample
        if (sample_map.find(randNum)->second == false) { //if the sample is not used to make a sublist,
            SublistNum = SublistNum % n + 1; //put it in a sublist (number between sublist #1 ~ #total number of sublist)
            if (sublist.count(SublistNum) > 0) { //if the sublist is not empty
                sublist.find(SublistNum)->second.push_back(randNum); //push the sample number in the back of the sublist
                countNum++; //now the number of samples used in making a sublist increased
            } else { //if the sublist is empty
                list<unsigned int> sublist_initialize;
                sublist_initialize.push_back(randNum);
                sublist[SublistNum] = sublist_initialize; //add a new list to the map
                countNum++; //now the number of samples used in making a sublist increased
            }
        }
    }
    for (unsigned int i = 1; i <= n; i++) { //for n times
        list<unsigned int> test_subset;
        list<unsigned int>::iterator L_iterator;
        for (L_iterator = sublist.find(i)->second.begin();
             L_iterator != sublist.find(i)->second.end(); L_iterator++) {
            test_subset.push_back(*L_iterator);//[MainLabProblem] one of the subset will be used as test set
        }
        list<unsigned int> train_subset;
        for (unsigned int j = 1; j <= Num_Total; j++) {
            train_subset.push_back(j);
        }
        for (L_iterator = test_subset.begin(); L_iterator != test_subset.end();
             L_iterator++) {
            train_subset.remove(*L_iterator);
        } //the other samples are used as train_subset
        Node node;
        node.data.sampleList = train_subset; //[MainLabProblem] set data in node
        node.data.clsGeneList = d.clsGeneList; //[MainLabProblem] set data in node
        DecisionTree::makeDecisionTree(&node); //[MainLabProblem] using the DEG list and train subsets, construct a decision tree
        Accuracy_Avg += getAccuracy(test_subset,&node); //[MainLabProblem] determine the accuarcy of the decision tree, and add it to the variable "Accuracy_Avg"
    }
    return Accuracy_Avg / n; //divide the variable with number of sets that we've done
};

int main(){
    /* 1. Read data from file (Prelab) */
    DecisionTree dt;
    DecisionTree dt_test;
    dt.readDataFromFile("Lab10_GSE13164_train.txt"); // use given input file
    dt_test.readDataFromFile("Lab10_GSE13164_test.txt"); // use given input file
    list<string> deg = dt.readDEGFromFile("Lab10_GSE13164_DEG.txt"); // use given input file

    /*************************************************************************************/
    // This is Test for Prelab. Run this part and check it to TA before starting main lab.
    // After that you can erase this part
    dt.testReadFile();
    dt.testDEGList(deg);

    list<unsigned int> l;
    l.push_back(1);   l.push_back(2);   l.push_back(3);
    l.push_back(36);   l.push_back(37);   l.push_back(38);

    list<string>::iterator it = deg.begin();
    double tmp_cov = dt.findCutoffValue(*it,l);
    dt.testCutoff(*it, l);
    cout << tmp_cov << endl;

    list<double> cov_l;
    cov_l.push_back(tmp_cov);
    cov_l.push_back(tmp_cov*0.5);
    cov_l.push_back(tmp_cov*1.5);
    dt.testInfoGain(*it, cov_l, l);
    /*************************************************************************************/

    /* 2. Prepare data part of root node for a parameter of function that generates decision tree */
    cout << "\nLab10 Main Lab Results" << endl;
    Node n;
    map<unsigned int, bool>::iterator it3;
    for(it3 = dt.sampleCls.begin(); it3 != dt.sampleCls.end(); it3++){
        n.data.sampleList.push_back(it3->first);
    }
    n.data.clsGeneList=deg;
    cout << "#2 done" << endl;


    /* 3. Generate Decision Tree & Draw the tree from root to reaf */
    cout << "\nDecision Tree & Draw the tree from root to reaf" << endl;
    dt.makeDecisionTree(&n);
    dt.drawGraph(&n);
    cout << "#3 done" << endl;

    /* 4. Prepare whole data for cross validation */
    Data d;
    d.clsGeneList = n.data.clsGeneList;
    d.sampleList = n.data.sampleList;
    cout << "\nAccuracy for all the sample: "
         << dt.getAccuracy(n.data.sampleList, &n) << endl;

    /* 5. Print 10-fold cross validation result */
    cout << "\n10-fold cross validation result :" 
        << dt.nFoldCV(10, d) << endl;

    /* 6. Print Accuracy for test set */
    list<unsigned int> test_sampleList;
    cout << "\nPrediction result of test set samples" << endl;
    for(it3 = dt_test.sampleCls.begin(); it3 != dt_test.sampleCls.end(); it3++){
        test_sampleList.push_back(it3->first);
        
    /* 7. Print Result Class for test set */
        cout << "\nTest sample class results : " <<  "(Sample Number:Result Class Number)  " << test_sampleList.size() << " :" << dt_test.predSample(test_sampleList.size(),&n) ;
    }
    cout << "\nAccuracy for test set : " << dt_test.getAccuracy(test_sampleList, &n ) << endl;

            
}




