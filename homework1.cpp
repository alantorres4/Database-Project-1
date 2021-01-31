#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

using namespace std;

string csvFile;
string prefixName;

bool OPEN_DATABASE = false;
ifstream Din;
ofstream DoutConfig;
ofstream DoutData;
ifstream DinTest;
ifstream DinData;

// 'string OTHER' is TEMPORARY FOR NOW, TAKES THE 'TYPE' AND 'NAME' AS ONE WHOLE VARIABLE
string OTHER;

string csvConfigFile;
string csvDataFile;
string csvFinal;

string ID;
string REGION;
string STATE;
string CODE;
string NAME;
string TYPE;
string VISITORS;

int numberOfFiles;

const int TOTAL_MAX = 157; // the total number of characters each line should have

// this is where I have the max length that each field should have
const int ID_MAX = 8;
const int REGION_MAX = 3;
const int STATE_MAX = 3;
const int CODE_MAX = 5;
const int NAMES_MAX = 86;
const int TYPE_MAX = 40; 
const int VISITORS_MAX = 12; 

string BLANK_RECORD = "                                                                                                                                                             "; // this SHOULD be 157 characters of white space

int ID_RECORD; // used for displayRecord()




void padRecords(string &ID, string &REGION, string &STATE, string &CODE, string &NAME, string &TYPE, string &VISITORS){
        int appendNum;

        appendNum = ID_MAX - ID.length();
        for(int a=0; a<appendNum; a++)
            ID = ID + " ";

        appendNum = REGION_MAX - REGION.length();
        for(int b=0; b<appendNum; b++)
            REGION = REGION + " ";

        appendNum = STATE_MAX - STATE.length();
        for(int c=0; c<appendNum; c++)
            STATE = STATE + " ";

        appendNum = CODE_MAX - CODE.length();
        for(int d=0; d<appendNum; d++)
            CODE = CODE + " ";

        appendNum = NAMES_MAX - NAME.length();
        for(int e=0; e<appendNum; e++)
            NAME = NAME + " ";


        appendNum = TYPE_MAX - TYPE.length();
        for(int f=0; f<appendNum; f++)
            TYPE = TYPE + " ";

        appendNum = VISITORS_MAX - VISITORS.length();
        for(int g=0; g<appendNum; g++)
            VISITORS = VISITORS + " ";
}


void createDatabase(){
    // TODO
    if(!OPEN_DATABASE){
        cout << "\n [] Enter the name of a .csv file (Exclude extension, e.g., 'Parks'): ";
        cin >> csvFile;
            // HERE MIGHT NEED TO ADD SOME ERROR CHECKING TO MAKE SURE THE .CSV FILE EXISTS IN THE DIRECTORY
        cout << "\n + Two new files have been made: " << "\n    []" << csvFile << ".config" << "\n    []" << csvFile << ".data\n";
        // Read and convert the .csv file into two files: Parks.config and Parks.data
        // This can be specific to a csv file that has exactly the 7 fields in our datafile
        // YOu may assume that the records in the .csv file are sorted in order of the primary key
            // Parks.config: contains the number of records in the file, describes the names, sizes of the fields in order, anything els you want. The first field is assumed to be the key
            // Parks.data: contains the data records, one per line, with fixed size fields. You may use any separator you want (or no separator)
        csvConfigFile = csvFile + ".config";
        csvDataFile = csvFile + ".data";
        csvFinal = csvFile + ".csv";

        // HERE WE OPEN OUR FILES
        DoutConfig.open(csvConfigFile);
        DoutData.open(csvDataFile);
        Din.open(csvFinal);

        // THIS IS WHERE WE ADD INFO TO OUR .CONFIG FILE
        string firstLine;
        string headers;

        getline(Din, firstLine);
        stringstream s(firstLine);

        vector<string> firstRow;

        while(getline(s, headers, ',')){
            firstRow.push_back(headers);
        }

        ID = firstRow[0];
        REGION = firstRow[1];
        STATE = firstRow[2];
        CODE = firstRow[3];
        NAME = firstRow[4];
        TYPE = firstRow[5];
        VISITORS = firstRow[6];

        DoutConfig << ID << "||" << REGION << "||" << STATE << "||" << CODE << "||"<< VISITORS << "||" << TYPE << "||" << NAME << endl;
        firstRow.clear(); // clear the vector to save space

        // THIS IS WHERE WE ADD INFO TO OUR .DATA FILE
        string currentLine = "";
        vector<string> FieldValues;
        while(!Din.eof()){
            string fields;

            getline(Din, currentLine);
            stringstream s(currentLine);

            while(getline(s, fields, ',')){
                FieldValues.push_back(fields);
            }

            ID = FieldValues[0];
            REGION = FieldValues[1];
            STATE = FieldValues[2];
            CODE = FieldValues[3];
            NAME = FieldValues[4];
            TYPE = FieldValues[5];
            VISITORS = FieldValues[6];

        // Here we will add ' 's to each field
            padRecords(ID, REGION, STATE, CODE, NAME, TYPE, VISITORS);


        // DoutData into the .data file and add a blank record to the next line
            DoutData << ID << REGION << STATE << CODE << VISITORS << TYPE << NAME << endl << BLANK_RECORD << endl;

            currentLine = "";
            FieldValues.clear();
            numberOfFiles = numberOfFiles + 1; // here we keep track of the number of real records to add to our .CONFIG file
        }

        // .data file is being wierd and display two extra ghostly data records, so just subtract two from numberOfFiles
        numberOfFiles = numberOfFiles - 2;

        DoutConfig << numberOfFiles; // here we add the real record number to our config file
        OPEN_DATABASE = true;
        prefixName = csvFile; // this is as if you had done openDatabase() on the freshly made files.
    }
    else
        cout << "\n - [Please close your current database first before creating a new one]\n";
}

void openDatabase(){
    if (!OPEN_DATABASE){
        cout << "\n [] What's the prefix for your file? (Exclude extension, e.g., 'Parks')\n";
        cin >> prefixName;

        csvConfigFile = prefixName + ".config";
        csvDataFile = prefixName + ".data";
        csvFinal = prefixName + ".csv";

        Din.open(csvFinal);
        if(!Din)
            cout << " - ERROR. Could not open " << csvFinal << ".\n\n";
        else {
            DinTest.open(csvConfigFile);
//          DoutConfig.open(csvConfigFile);
            DoutData.open(csvDataFile, std::ios_base::app);
            Din.open(csvFinal);

            // I JUST DIN'd THE FIRST AND SECOND LINE SEPARATELY AND IGNORED THE FIRST LINE of the config file
            string test;
            string test2;
            DinTest >> test;
            DinTest >> test2;

            numberOfFiles = stoi(test2); // STOI() HAS SOME TROUBLE, I'M NOT TOO ENTIRELY SURE WHY...
            cout << " + NUMBER OF FILES: " << numberOfFiles << endl;

            DinTest.close();

            OPEN_DATABASE = true;
            return;
        }
    }
    else{
        cout << "\n - [Please close your current database first before opening a new one]\n";
    }

}


void closeDatabase(){
    if(!OPEN_DATABASE)
        cout << "\n - [No database currently open]\n";
    else{
        cout << "\n + [Successfully closed database]\n";
        OPEN_DATABASE = false;

        Din.close();
        DoutConfig.close();
        DoutData.close();
    }
}

bool getRecord(ifstream &DinData, int ID_RECORD, const int middle, string &middleID){
    bool SUCCESS = false;
    if((0 <= middle) && (middle < numberOfFiles)){
        DinData.seekg(middle * TOTAL_MAX, ios::beg);
        DinData >> ID >> REGION >> STATE >> CODE >> VISITORS;
        getline(DinData, OTHER);

        // THESE COUT's ARE USED FOR TESTING PURPOSES- to see where stuff breaks 
        // cout << "      ID = " << ID << "\n      REGION = " << REGION << "\n      STATE = " << STATE << "\n      CODE = " << CODE << "\n      VISITORS = " << VISITORS << "\n      OTHER: " << OTHER  << endl << endl;
        // cout << "      MIDDLE: " << middle << endl;
        middleID = ID;
        
        
        SUCCESS = true;
    }
    else
        cout << " - [Record " << ID_RECORD << " out of range]\n";

    return SUCCESS;
}

void displayRecord(){
    cout << "Please enter ID for the record: ";
    cin >> ID_RECORD;


    DinData.open(csvDataFile);
    // binary search part
    int low = 0;
    int high = numberOfFiles - 1;
    int middle = 0;
    string middleID;
    bool Failure = false;

    bool Found = false;
    while(!Found && (high >= low) && !Failure)
    {
        middle = (low + high) / 2;
        if(getRecord(DinData, ID_RECORD, middle, middleID))
        {
            if(stoi(middleID) == ID_RECORD){
                Found = true;
            }
            else if (stoi(middleID) < ID_RECORD){
                low = middle + 1;

                // THESE COUT's ARE USED FOR TESTING PURPOSES
                // cout << " -> LOW" << endl;
                // cout << "    ID_RECORD: " << ID_RECORD << endl;
            }
            else{
                high = middle - 1;

                // TESTING PURPOSES
                // cout << " -> HIGH" << endl;
                // cout << "    ID_RECORD: " << ID_RECORD << endl;
            }
        }
        else
        {
            cout << " - [Could not get record " << middle << endl;
            Failure = true;
        }
    }

    if (Found){
        cout << "ID: " << ID_RECORD << " found at record " << middle << ": \n";
        cout << "ID: " << ID_RECORD << "\nREGION: " << REGION << "\nSTATE: " << STATE << "\nCODE: " << CODE <<  "\nVISITORS: " << VISITORS  << "\nOTHER: " << OTHER << endl;
    }
    else{
        cout << " - [Could not find record]\n";
    }
    DinData.close();
}


void updateRecord(){
    // TODO
    cout << "\n--updating record--\n";
    // As a first step, you have to find the record using the same "find" routine as 4
    // Then you should display the contents and allow updates in a specified field
    // DON'T allow the updating of the primary key
}

void createReport(){
    // TODO
    cout << "\n--creating report--\n";
    // This should create a "human" readable file in ASCII form, which should display the first TEN records nicely formatted, in sorted order by primary key
}

void addRecord(){
    // TODO
    cout << "\n--adding record--\n";
    // Add the record at the database by overwriting a blank record
    // If there is no blank record at the required location (i.e. it's already been filled), the database is reorganized to reinsert blank records after each real record
    // You should NOT need to have more than one or two records in memory at a time to accomplish this
}

void deleteRecord(){
    // TODO
    cout << "\n--deleting record--\n";
    // Delete the record at the database
    // As records are deleted, they are logically removed from the file (most likely just marked as "missing")
    // The "missing" records should be removed when the file is reorganized while reinserting blank records
}


int main(){
    cout << "\n\n============================\nNAME: Alan Torres\nID:  010840328\n----------------------------\n\n";

    bool dontLeave = true;
    int CHOICE = 0;

    while(dontLeave){
        while(CHOICE < 1 || CHOICE > 9){
            cout << "\nPlease choose an option (1-9):\n";
            cout << "   [1] Create new database\n   [2] Open database\n   [3] Close database\n   [4] Display record\n   [5] Update record\n   [6] Create report\n   [7] Add record\n   [8] Delete record\n   [9] Quit\n\n";
            cin >> CHOICE;
        }
        switch(CHOICE){
            case 1:
                createDatabase();
                break;
            case 2:
                openDatabase();
                break;
            case 3:
                closeDatabase();
                break;
            case 4:
                displayRecord();
                break;
            case 5:
                updateRecord();
                break;
            case 6:
                createReport();
                break;
            case 7:
                addRecord();
                break;
            case 8:
                deleteRecord();
                break;
            case 9:
                dontLeave = false;
                break;
            default:
                dontLeave = false;
        }
        CHOICE = 0;
    }
    cout << "\n\nGOODBYE.\n----------------------------\n============================\n\n";
    return 0;
}