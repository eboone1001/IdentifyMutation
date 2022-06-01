//
// Created by Eric Boone on 5/24/2022. Takes input in the form of
//
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<list>
using namespace std;

const string NUMERIC = "0123456789";
const string BASES = "ATGC-NKWYRTM";
map<string, string> codon_dict;
map<string, string> database_dict;


void init_codon_dictionary() {
    codon_dict["ATT"] = "I";
    codon_dict["ATC"] = "I";
    codon_dict["ATA"] = "I";
    codon_dict["ATG"] = "M";
    codon_dict["ACT"] = "T";
    codon_dict["ACC"] = "T";
    codon_dict["ACA"] = "T";
    codon_dict["ACG"] = "T";
    codon_dict["AAT"] = "N";
    codon_dict["AAC"] = "T";
    codon_dict["AAA"] = "K";
    codon_dict["AAG"] = "K";
    codon_dict["AGT"] = "S";
    codon_dict["AGC"] = "S";
    codon_dict["AGA"] = "R";
    codon_dict["AGG"] = "R";
    codon_dict["GTT"] = "V";
    codon_dict["GTC"] = "V";
    codon_dict["GTG"] = "V";
    codon_dict["GTA"] = "V";
    codon_dict["GCT"] = "A";
    codon_dict["GCC"] = "A";
    codon_dict["GCA"] = "A";
    codon_dict["GCG"] = "A";
    codon_dict["GAT"] = "D";
    codon_dict["GAC"] = "D";
    codon_dict["GAA"] = "E";
    codon_dict["GAG"] = "E";
    codon_dict["GGT"] = "G";
    codon_dict["GGC"] = "G";
    codon_dict["GGG"] = "G";
    codon_dict["GGA"] = "G";
    codon_dict["TTT"] = "F";
    codon_dict["TTC"] = "F";
    codon_dict["TTG"] = "L";
    codon_dict["TTA"] = "L";
    codon_dict["TCT"] = "S";
    codon_dict["TCC"] = "S";
    codon_dict["TCA"] = "S";
    codon_dict["TCG"] = "S";
    codon_dict["TAT"] = "Y";
    codon_dict["TAC"] = "Y";
    codon_dict["TAA"] = "STOP";
    codon_dict["TAG"] = "STOP";
    codon_dict["TGT"] = "C";
    codon_dict["TGC"] = "C";
    codon_dict["TGG"] = "STOP";
    codon_dict["TGA"] = "W";
    codon_dict["CTT"] = "L";
    codon_dict["CTC"] = "L";
    codon_dict["CTG"] = "L";
    codon_dict["CTA"] = "L";
    codon_dict["CCT"] = "P";
    codon_dict["CCC"] = "P";
    codon_dict["CCA"] = "P";
    codon_dict["CCG"] = "P";
    codon_dict["CAT"] = "H";
    codon_dict["CAC"] = "H";
    codon_dict["CAA"] = "Q";
    codon_dict["CAG"] = "Q";
    codon_dict["CGT"] = "R";
    codon_dict["CGC"] = "R";
    codon_dict["CGG"] = "R";
    codon_dict["CGA"] = "R";
}

//TODO: Move to helper .cpp file
list<string> split (string str, char separator) {
    list<string> strings;
    int currIndex = 0, i = 0;
    int startIndex = 0, endIndex = 0;
    while (i <= str.length()) {
        if (str[i] == separator || i == str.length()) {
            endIndex = i;
            string subStr = "";
            subStr.append(str, startIndex, endIndex - startIndex);
            strings.push_back(subStr);
            currIndex += 1;
            startIndex = endIndex + 1;
        }
        i++;
    }

    return strings;
}

void init_database_dictionary(string filepath) {
    std::ifstream inputFile(filepath);
    string line;
    string name;
    string strain;
    string protein;
    string mutation;


    if (inputFile.is_open()) {
        while (getline(inputFile, line)) {
            int split1 = line.find_first_of('\t');
            int split2 = line.find_first_of('|');
            name = line.substr(0, split1);
            strain = name.substr(0, split2);
            protein =  name.substr(split2 + 1);
            getline(inputFile,line);
            //cout << protein << ' ' << line.length() << endl;
            database_dict[protein] = line;
        }
    }
}

string decode_codon(string codon) {

    if (codon.length() != 3) {
        throw std::logic_error("String isn't a codon: " + codon);
    }

    if (codon == "---") {
        return "DEL";
    }
    else if (codon.find('-') != string::npos) {
        return codon_dict[codon] + "(FRAMESHIFT)";
    }
    return codon_dict[codon];
}

string fillCodon(string queryC, string subjC) {

    for (int i = 0; i < 3; i++) {
        if (queryC[i] == '@') {
            queryC[i] = subjC[i];
        }
    }
    return queryC;
}

// TODO: database as a string works but memory inefficient, change to use pointer instead.
tuple<list<string>,list<string>> getCodons(int startPosition, string mutation, string database) {
    int frame = startPosition % 3;
    int currentFrame;
    int pos = startPosition - frame;
    tuple<list<string>,list<string>> codons;
    string subjectCodon;
    string queryCodon = "@@@";
    int shift = 0;

    do {

        currentFrame = (frame + shift) % 3;
        if (queryCodon == "@@@") {
            subjectCodon = database.substr(pos, 3);
            get<1>(codons).push_back(subjectCodon);
            pos = pos + 3;
        }

        queryCodon[currentFrame] = mutation[0];
        if (currentFrame == 2) {
            queryCodon = fillCodon(queryCodon, subjectCodon);
        }
        if (queryCodon.find('@') == string::npos) {
            get<0>(codons).push_back(queryCodon);
            queryCodon = "@@@";
        }
        mutation = mutation.substr(2);
        shift++;
    } while (mutation.length() > 0);

    if (queryCodon != "@@@") {
        queryCodon = fillCodon(queryCodon, subjectCodon);
        get<0>(codons).push_back(queryCodon);
    }
    return codons;
}

string parseMutation(string blastAlignment, string proteinName) {
    string finalMutations;
    int db_position = 0;
    int matched_bases = 0;
    string database = database_dict[proteinName];
    string mutations;
    std::size_t pos = blastAlignment.find_first_of(BASES);

    while (pos!=std::string::npos) {
        // Pull out integer representing number of matching bases before mutation
        matched_bases = stoi(blastAlignment.substr(0, pos));
        blastAlignment = blastAlignment.substr(pos);
        pos = blastAlignment.find_first_of(NUMERIC);
        // Extract string depicting mutation
        mutations = blastAlignment.substr(0, pos);
        blastAlignment = blastAlignment.substr(pos);
        pos = blastAlignment.find_first_of(BASES);
        db_position += matched_bases;
        tuple<list<string>,list<string>> codons = getCodons(db_position, mutations, database);
        list<string>& queryCodons  = get<0>(codons);
        list<string>& subjectCodons  = get<1>(codons);

        while (!queryCodons.empty()) {
            int codon_pos = db_position/3;
            string queryAA = decode_codon(queryCodons.front());
            queryCodons.pop_front();
            string subjectAA = decode_codon(subjectCodons.front());
            subjectCodons.pop_front();

            if (subjectAA != queryAA && queryAA != "") {

                finalMutations +=  proteinName + ':' + subjectAA + to_string(codon_pos + 1) + queryAA + ',';
            }
            codon_pos++;
        }
        db_position += mutations.length()/2;

        // cout << matched_bases << mutations << endl;
    }
    return finalMutations.substr(0, finalMutations.length() -1);
}

string read_file(const std::string& filepath) {

    std::ifstream inputFile(filepath);
    string outFilePath = filepath.substr(0, filepath.find_last_of('.')) + "_results.txt";
    std::ofstream outputFile(outFilePath);
    //inputFile.open(filepath.c_str(), ios::in);
    string line;
    string relevant;
    string name;
    string strain;
    string protein;
    string mutation;

    if (inputFile.is_open()) {
        while(getline(inputFile, line)) {

            list<string> strings = split(line, '\t');
            protein = split(strings.front(), '|').back();
            strings.pop_front();
            strain = strings.front();
            mutation = strings.back();
            cout << strain << protein << mutation << endl;

            /*
            int split = line.find_first_of('|');
            relevant = line.substr(split + 1);

            cout << relevant << split << endl;
            split = relevant.find_first_of('\t');
            protein =  relevant.substr(0, split);

            cout << relevant << protein << endl;
            split = relevant.find_first_of('\t');
            relevant = relevant.substr(split);

            cout << relevant << endl;
            split = relevant.find_first_of('\t');
            strain = relevant.substr(0, split);
            cout << strain << endl;
            mutation = relevant.substr(split);// relevant.find_first_of('\t'));
            cout << "Mutation: " << mutation << endl;*/


            string mutAnnotation = parseMutation(mutation, protein);
            if (mutAnnotation != "") {
                outputFile << strain << " " << mutAnnotation << endl;
            }

        }
    }
    inputFile.close();
    outputFile.close();

    return "exiting successfully";
}

int main(int argc, char* argv[]) {
    init_codon_dictionary();
    init_database_dictionary("../protein_database_mod.fasta");
    //cout << parse_mutations("../test_output.txt");
    //parse_mutation("897TA1873TC4502CT5943");
    //parse_mutation("605-T-A-G-G-G-G-A-A-C-T-T-C-T-C-C-T-G-C637");
    //getCodons(897, "TA");

    read_file("../allgenes_mut.txt");
    return 0;
}

