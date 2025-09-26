//Line Object Deffinition
//v1.0.0

#define line_h

class dataLine
{
	private:
		//Variables
		std::string head, grab;
		int* longID=new int [16];
		int* shortID=new int [16];
		int* chantID=new int [16];
		int* trigtID=new int [2];
		//Functions
		void build()
		{
			for(int i=0; i<16; i++)
			{
				longID[i]=-1;
				shortID[i]=-1;
				chantID[i]=-1;
			}
			for(int i=0; i<2; i++)
			{
				trigtID[i]=-1;
			}
		}
		
		bool checkHead(std::string input)
		{
			head=input;
			int i=0, k=0;
			bool stop=true;
			grab="";
			while(i<head.length())
			{
				if(head[i]=='.' and stop)
				{
					if(grab=="mdpp16_qdc")
					{
						stop=false;
					}
					else
					{
						std::cout << "\n--->Checkpoint Failed at Position #" << k << "!\n\tIs this a MDPP16-QDC?\n\n";
						return false;
					}
				}
				if(head[i]==',')
				{
					k++;
					grab="";
					stop=true;
				}
				else
				{
					grab+=head[i];
				}
				i++;
			}
			std::cout << "\n--->Checkpoint Success! Head Line Confirmed!\n\n";
			return true;
		}
		
		int pull(int k)
		{
			int i=0, j=0;
			int a, b;
			while(j<k-1)
			{
				if(grab[i]==',') {j++;}
				i++;
			}
			a=i;
			if(grab[a]==',') {return -1;}
			while(j<k)
			{
				i++;
				if(grab[i]==',') {j++;}
			}
			return std::stoi(grab.substr(a,b-a+1));
		}
	
	public:
		//Members
		bool status;
		
		//Constructor
		dataLine(std::string firstLine)
		{
			build();
			status=checkHead(firstLine);
		}
		
		//Functions
		bool getStatus()
		{
			return status;
		}
		
		void organize()
		{
			int i=0, k=0;
			bool stop=false;
			grab="";
			while(i<head.length())
			{
				if(k==0 and not stop or head[i-1]==',') {i+=11; stop=true;}
				if(head[i]=='.' and stop)
				{
					if(grab=="integration_long")
					{
						grab="";
						i++;
						while(head[i]!=',') {grab+=head[i]; i++;}
						k++;
						longID[std::stoi(grab)]=k;
						grab="";
						stop=false;
					}
					if(grab=="integration_short")
					{
						grab="";
						i++;
						while(head[i]!=',') {grab+=head[i]; i++;}
						k++;
						shortID[std::stoi(grab)]=k;
						grab="";
						stop=false;
					}
					if(grab=="channel_time")
					{
						grab="";
						i++;
						while(head[i]!=',') {grab+=head[i]; i++;}
						k++;
						chantID[std::stoi(grab)]=k;
						grab="";
						stop=false;
					}
					if(grab=="trigger_time")
					{
						grab="";
						i++;
						while(head[i]!=',') {grab+=head[i]; i++;}
						k++;
						trigtID[std::stoi(grab)]=k;
						grab="";
						stop=false;
					}
				}
				if(head[i]!=','){grab+=head[i];}
				i++;
			}
			return;
		}
		
		void setLine(std::string input)
		{
			grab=input;
			return;
		}
		
		bool trigHit()
		{
			int i=0, k=0;
			while(k<trigtID[0]-1)
			{
				if(grab[i]==','){k++;}
				i++;
			}
			if(grab[i+1]==',')
			{
				return false;
			}
			else
			{
				return true;
			}
			
		}
		
		bool evtHit(int channel)
		{
			int i=0, k=0;
			while(k<longID[channel]-1)
			{
				if(grab[i]==','){k++;}
				i++;
			}
			i--;
			if(grab[i+1]==',')
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		
		int getLong(int channel)
		{
			return pull(longID[channel]);
		}
		
		int getShort(int channel)
		{
			return pull(shortID[channel]);
		}
		
		int getChan_Time(int channel)
		{
			return pull(chantID[channel]);
		}
		
		int getTrig_Time(int channel)
		{
			if(channel>1)
			{
				return -1;
			}
			else
			{
				return pull(trigtID[channel]);
			}
		}
		
		//Destructor
		~dataLine()
		{
			delete[] longID;
			delete[] shortID;
			delete[] chantID;
			delete[] trigtID;
			longID=nullptr;
			shortID=nullptr;
			chantID=nullptr;
			trigtID=nullptr;
		}
};
