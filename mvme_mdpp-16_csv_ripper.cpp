//Organization & Output Code for MVME .csv Data File Output. Version: 1.0.0
// Program by: Cade Saugier, INPP EAL Ohio University

/*
This C++ code is open source.
Do whatever you want with it.
I don't mind.

For support: Go to room 115 EAL.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>

#include "./include/histxy.h"
#include "./include/line.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::to_string;
using std::stoi;
using std::cout;
using std::endl;



///////////////////////////////////// Build Functions ////////////////////////////////////
int getPSD(int s, int l)
{
	if(l==0 or l==-1) {return -1;}
	float longVal = l, shortVal = s;

/*################# PSD CALCULATION ##################*/
	float thisPSD = shortVal/longVal;
/*####################################################*/

	if(thisPSD>=1)
	{
		return -1;
	}
	return static_cast<int>(16*4096*thisPSD);
}

int getTOF(int chanTime, int trigTime)
{
	int tof;

/*####################### TOF CALCULATION ########################*/
	int tdc_res = 0;	//Value from MVME Module Init
	int freq_div = 2;	//Value from Control Pannel f/n
/*################################################################*/

	if(trigTime==-1 or chanTime==-1)
	{
		return -1;
	}
	tof=(8192*freq_div/pow(2,tdc_res));
	if(trigTime-chanTime<0) {return -1;}
	tof-=trigTime-chanTime;
	if(tof<0 or tof>=65536)
	{
		return -1;
	}
	else
	{
		return tof;
	}
}

void update(int a,int b,int c)
{
	cout << "\n===" << c << " Recorded Lines";
	cout << "\n==" << b << " Checked Lines";
	cout << "\n=" << a << " Total Events";
	cout << endl;
	return;
}

/////////////////////////////////////// Build Main ///////////////////////////////////////
int main(int argc, char* argv[])
{
	cout << "\n<><><><><><><><><><><><><> MVME MDPP-16 CSV RIPPER <><><><><><><><><><><><><>\n";
	cout << "\t--->Version 1.0.0\n";
	cout << "\t--->By: Cade S.\n\n";
	
	//Check Terminal Inputs $> hist evt [#,#,#] file.csv log
		int pos=0;
		//Make Histogram / Event /Both
		bool histMake, evtMake;
		string input1=argv[1], input2=argv[2];
		if(input1=="hist" or input2=="hist") {histMake=true; pos++;}
		else {histMake=false;}
		if(input1=="evt" or input2=="evt") {evtMake=true; pos++;}
		else {evtMake=false;}
		//Data to Record
		pos++;
		if(pos==1)
		{
			cout << "--->Histogram / Event Data Not Requested...\n";
			exit(0);
		}
		bool dataRec[16];
		string input3=argv[pos];
		if(input3=="[]")
		{
			for(int i=0; i<16; i++)
			{
				dataRec[i]=true;
			}
		}
		else
		{
			for(int i=0; i<16; i++)
			{
				dataRec[i]=false;
			}
			int k=1, j, val;
			while(k<input3.length()-1)
			{
				j=0;
				while(input3[k+j]!=',' and input3[k+j]!=']')
				{
					j++;
				}
				val=stoi(input3.substr(k,j));
				if(val>15 or val<0)
				{
					cout << "--->Non-Physical Channel Input #"+to_string(val) << endl;
				}
				else
				{
					dataRec[val]=true;
				}
				k=k+j+1;
			}
		}
		pos++;
		cout << "\n---Outputs to Build---\n" << "-------------------------\n";
		cout << "Histograms\t";
		if(histMake) {cout << "YES\n";}
		else {cout << "NO\n";}
		cout << "Event File\t";
		if(evtMake) {cout << "YES\n";}
		else {cout << "NO\n";}
		cout << "-------------------------\n";
		cout << "\n---Channels to Record---\n" << "-------------------------\n";
		for(int i=0; i<16; i++)
		{
			cout << "#"+to_string(i)+"\t";
			if(dataRec[i]) {cout << "YES\n";}
			else {cout << "NO\n";}
		}
		cout << "-------------------------\n";
		//Record Data File Name and Check File Type
		string dataFilePath = argv[pos];
		if(dataFilePath.substr(dataFilePath.length()-4,4)!=".csv")
		{
			cout << "--->File Type Not .csv!\n";
			exit(0);
		}
		ifstream dataFile;
		dataFile.open(dataFilePath);
		if(dataFile.is_open())
		{
			cout << "\n--->Data File Open at " << dataFilePath << endl;
		}
		else
		{
			cout << "\n--->Error Opening File!\n";
			exit(0);
		}
		string dataFileName;
		int fileMarker, lastDir=0, val=0;
		for(int i=0; i<dataFilePath.length(); i++)
		{
			if(dataFilePath[i]=='/') {lastDir=i; val=1;}
			if(dataFilePath[i]=='.') {fileMarker=i;}
		}
		dataFileName=dataFilePath.substr(lastDir+val,fileMarker-lastDir-val);
		//Log File Argument
		ofstream logFile;
		bool logMake=false;
		pos++;
		try
		{
			string input4 = argv[pos];
			if(input4=="log")
			{
				cout << "\n   WARNING!   \n**************\n--->You are about to make a .log file.\n" << "--->This file may become extremely large.\n" << "--->File size may exceed input data file size.\n";
				string ans;
				cout << "\n----->Do you wish to continue? (y/n): ";
				std::cin >> ans;
				if(ans=="y" or ans=="Y") {logMake=true;}
				else {logMake=false; dataFile.close(); cout << "\n--->Data File Closed at " << dataFilePath << endl << "--->Process Aborted\n"; exit(0);}
			}
		}
		catch (std::logic_error) {}
		if(logMake)
		{
			logFile.open("./"+dataFileName+"_rip.log");
			logFile << "START\n";
		}
		//Ask For All Recordings
		bool takeAll=false;
		if(logMake) {pos++;}
		try
		{
			string input5 = argv[pos];
			if(input5=="all")
			{
				cout << "++++++++++++++++++++++++++++++++++++++\n" << "*      ALL DATA ARGUMENT CALLED      *\n" << "++++++++++++++++++++++++++++++++++++++\n" << "--->Full Input To Be Recorded!\n--->Output Histogram Sums Will No Longer Match!\n";
				takeAll=true;
			}
		}
		catch (std::logic_error) {}
		
	//Get Head Line
		string read;
		dataFile >> read;
	
	//Make Output Directory Structure
		cout << "\n--->Making Output Directory Structure...\n";
		system(("mkdir ./output_"+dataFileName).c_str());
		for(int i=0; i<16; i++)
		{
			if(dataRec[i])
			{
				system(("mkdir ./output_"+dataFileName+"/data_"+to_string(i)).c_str());
			}
		}
	
	//Build Required Objects
		//Histogram Raw
		histo longHist[16];
		histo shortHist[16];
		histo chantHist[16];
		histo trigtHist[2];
		//Histogram Calculation
		histo psdHist[16];
		histo tofHist[16];
		//Event File
		ofstream evtFile[16];
		if(evtMake)
		{
			for(int i=0; i<16; i++)
			{
				if(dataRec[i])
				{
					evtFile[i].open("./"+dataFileName+"_"+to_string(i)+".evtln");
					evtFile[i] << ">> short,long,psd,tof,chan_time\n";
					cout << "\n--->Event File Created #"+to_string(i)+"\n";
				}
			}
		}
		//Line Object
		dataLine eventLine(read);
		if(logMake) {logFile << "dataLine Built! Status=";}
		if(eventLine.getStatus())
		{
			if(logMake) {logFile << "PASS\n";}
			eventLine.organize();
		}
		else {logFile << "FAIL\n";}
	
	//Main Loop
		int raw_events=0;
		int timed_events=0;
		int recordings=0;
		int mult=1;
		int hold[4];
		int calc[2];
		if(logMake) {logFile << ">>Starting Main Loop!\n";}
		while(!dataFile.eof())
		{
			raw_events++;
			if(logMake) {logFile << to_string(raw_events)+": ";}
			if(raw_events==mult*1500000) {update(raw_events,timed_events,recordings); mult++;}
			dataFile >> read;
			if(read=="") {if(logMake) {logFile << "FAIL=EMPTY STRING! IS THIS THE END?";} break;}
			eventLine.setLine(read);
			if(eventLine.trigHit() or takeAll)
			{
				timed_events++;
				if(logMake) {logFile << "Line Accepted!\n";}
				hold[3]=eventLine.getTrig_Time(0);
				trigtHist[0].plug(hold[3]);
			}
			else {if(logMake) {logFile << "\n";} continue;}
			for(int i=0; i<16; i++)
			{
				if(dataRec[i] and eventLine.evtHit(i))
				{
					recordings++;
					hold[0]=eventLine.getLong(i);
					hold[1]=eventLine.getShort(i);
					hold[2]=eventLine.getChan_Time(i);
					if(logMake)
					{
						logFile << "\tFOUND CH.#" << to_string(i) << ": " 
							<< to_string(hold[0]) << ","
							<< to_string(hold[1]) << ","
							<< to_string(hold[2]) << ","
							<< to_string(hold[3]) << "\n";
					}
					if(hold[0]==-1 or hold[1]==-1 or hold[2]==-1 and !takeAll) {continue;}
					calc[0]=getPSD(hold[1],hold[0]);
					if(calc[0]==-1 and !takeAll) {continue;}
					calc[1]=getTOF(hold[2],hold[3]);
					if(calc[1]==-1 and !takeAll) {continue;}
					if(histMake)
					{
						longHist[i].plug(hold[0]);
						shortHist[i].plug(hold[1]);
						chantHist[i].plug(hold[2]);
						psdHist[i].plug(calc[0]);
						tofHist[i].plug(calc[1]);
					}
					if(evtMake)
					{
						evtFile[i] << hold[1] << "," << hold[0] << "," << calc[0] << "," << calc[1] << "," << hold[2] << "\n";
					}
				}
			}
		}
		if(logMake) {logFile << "\nDONE\n";}
	
	//Final Results
		cout << "\n#################################\n";
		cout << "Total Event Lines:\t" << raw_events << endl;
		cout << "Triggered Lines  :\t" << timed_events << endl;
		cout << "Saved Events     :\t" << recordings << endl;
	
	//Build Histogram Files
		cout << "\n\n--->Building Histogram Files...\n";
		ofstream longOut, shortOut, chantOut, psdOut, tofOut;
		string tagAll = "";
		if(takeAll)
		{
			tagAll="_all";
		}
		for(int i=0; i<16; i++)
		{
			if(dataRec[i])
			{
				longOut.open("./output_"+dataFileName+"/data_"+to_string(i)+"/long"+tagAll+".xy");
				shortOut.open("./output_"+dataFileName+"/data_"+to_string(i)+"/short"+tagAll+".xy");
				chantOut.open("./output_"+dataFileName+"/data_"+to_string(i)+"/chan_time"+tagAll+".xy");
				psdOut.open("./output_"+dataFileName+"/data_"+to_string(i)+"/psd"+tagAll+".xy");
				tofOut.open("./output_"+dataFileName+"/data_"+to_string(i)+"/tof"+tagAll+".xy");
				for(int k=0; k<65536; k++)
				{
					longOut << to_string(k) << "  " << to_string(longHist[i].get(k)) << "\n";
					shortOut << to_string(k) << "  " << to_string(shortHist[i].get(k)) << "\n";
					chantOut << to_string(k) << "  " << to_string(chantHist[i].get(k)) << "\n";
					psdOut << to_string(k) << "  " << to_string(psdHist[i].get(k)) << "\n";
					tofOut << to_string(k) << "  " << to_string(tofHist[i].get(k)) << "\n";
				}
				longOut.close();
				shortOut.close();
				chantOut.close();
				psdOut.close();
				tofOut.close();
			}
		}
		ofstream trig0Out, trig1Out;
		trig0Out.open("./output_"+dataFileName+"/trig0"+tagAll+".xy");
		trig1Out.open("./output_"+dataFileName+"/trig1"+tagAll+".xy");
		for(int k=0; k<65536; k++)
		{
			trig0Out << to_string(k) << "  " << to_string(trigtHist[0].get(k)) << "\n";
			trig1Out << to_string(k) << "  " << to_string(trigtHist[1].get(k)) << "\n";
		}
		trig0Out.close();
		trig1Out.close();
	
	//Close all File Objects
	dataFile.close();
	logFile.close();
	for(int i=0; i<16; i++)
	{
		evtFile[i].close();
	}
	
	return 0;
}
