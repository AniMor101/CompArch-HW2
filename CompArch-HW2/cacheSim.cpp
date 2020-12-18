/* 046267 Computer Architecture - Winter 20/21 - HW #2 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <cassert>
#include <math.h>
using std::FILE;
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
using std::stringstream;
using std::vector;

class TagLine{
private:
    uint32_t tag;
    bool valid;
    bool dirty;
public:
    //constructors:
    TagLine():tag(0),valid(false),dirty(false){};
    TagLine(uint32_t tag1):tag(tag1),valid(true),dirty(false){}

    //Setters:
    void setTag(uint32_t tag1){
        tag=tag1;
    }
    void setValid(bool valid1){
        valid=valid1;
    }
    void setDirty(bool dirty1){
        dirty=dirty1;
    }

    //Getters:
    uint32_t getTag(){
        return tag;
    }
    bool getValid(){
        return valid;
    }
    bool getDirty(){
        return dirty;
    }
    bool operator==(const TagLine& tagLine)const{
        /*if(&tagLine==this){
            return true;
        }
        else*/ if(tagLine.tag==this->tag){
            return true;
        }
        return false;
    }
    TagLine& operator=(const TagLine& tagLine){
        if(&tagLine!=this){
        this->tag=tagLine.tag;
        this->dirty=tagLine.dirty;
        this->valid=tagLine.valid;
        }
        return *this;
    }

};

class Set{
private:
    unsigned numberOfWays;
    vector<TagLine> ways;
    vector<int> waysCount;
    //unsigned missesCounter=0;
    //unsigned hitsCounter=0;
    //unsigned mainMemoryAccessesCounter=0;
public:
    //constructors:
    Set():numberOfWays(0),ways(),waysCount(){}
    Set(int numberOfWays1):numberOfWays(numberOfWays1){
        ways=std::vector<TagLine>(0);
        waysCount=std::vector<int>(0);
        for(int i=0 ; i<numberOfWays ; i++){
            waysCount.push_back(i);
        }
    }


    //Getters:
    int getNumberOfWays(){
        return numberOfWays;
    }
    vector<TagLine>& getWays(){
        return ways;
    }
    vector<int>& getWaysCount(){
        return waysCount;
    }
    void setWaysCount(unsigned index,unsigned newCount){
        waysCount[index]=newCount;
    }
    //More methods:
    bool operator==(const Set set)const{
        if (numberOfWays==set.numberOfWays
        && waysCount.size()==set.waysCount.size()
        && ways.size()==set.ways.size()){
            for(unsigned i=0 ; i<waysCount.size() ; i++){
                bool res=!(waysCount[i]==set.waysCount[i]);
                if(!(waysCount[i]==set.waysCount[i]))

                    return false;
            }

            for(unsigned i=0 ; i<ways.size() ; i++){
                bool res=!(ways[i]==set.ways[i]);
                if(!(ways[i]==set.ways[i]))
                    return false;
            }

        }
        return true;
    }
    int findTagLineIndex(uint32_t tag){
        TagLine tagLine=TagLine(tag);
        int size=ways.size();
        for(int i=0 ; i<size ; i++){
            if(ways.at(i)==tagLine && ways.at(i).getValid()==true){
                return i;
            }
        }
        return -1;
    }
    TagLine& findTagLine(uint32_t tag){
        TagLine tagLine=TagLine(tag);
        int size=ways.size();
        for(int i=0 ; i<size ; i++){
            if(ways.at(i)==tagLine)
                return ways.at(i);
        }
        //TagLine newTag=TagLine();
        return ways.at(0); //wont get there
    }

    int getLRU(){
        for(int i=0 ; i<numberOfWays ;i++){
            if(waysCount[i]==0){//this is LRU
                return i;
            }
        }
        return -1; //wouldnt get there.
    }
    TagLine& getLRUTagLine(){
        for(int i=0 ; i<numberOfWays ;i++){
            if(waysCount[i]==0){//this is LRU
                return ways[i];
            }
        }
        //TagLine tagline=TagLine(-1);
        return ways[0]; //wouldnt get there.
    }
    void updateWaysCountVec (unsigned index,unsigned tagLineCount){
        for(unsigned i=0 ; i<numberOfWays ; i++){
            if(waysCount[i]>tagLineCount){
                waysCount[i]--;
            }
        }
        waysCount[index]=numberOfWays-1;
    }
    void updateWaysCountVecAfterDelete (unsigned index,unsigned tagLineCount){
        for(unsigned i=0 ; i<numberOfWays ; i++){
            if(waysCount[i]<tagLineCount){
                waysCount[i]++;
            }
        }
        for(unsigned i=index ; i<numberOfWays-1 ;i++){
            waysCount[i]=waysCount[i+1];
        }
        waysCount[numberOfWays-1]=0;
    }
    /*void restoreTagline(TagLine tagline){
        if(tagline.getDirty()){
            //do something
            mainMemoryAccessesCounter++;
        }
    }*/
   /* void updateSetAfterRead(uint32_t tag){//also when tag is in the ways vector
        TagLine currTagLine=TagLine(tag);
        int index=findTagLineIndex(tag);
        int currLRUIndex=getLRU();

        if(index==-1){//not existed in ways which means read miss
            missesCounter++;
            mainMemoryAccessesCounter++;
            if(ways.size()==numberOfWays){//cache is full
                if(ways[currLRUIndex].getDirty()){
                    mainMemoryAccessesCounter++;
                }
                ways[currLRUIndex]=currTagLine;
                updateWaysCountVec(currLRUIndex,0);
            }else{//cache isnt full
                ways.push_back(currTagLine);
                updateWaysCountVec(ways.size()-1,0);
            }
        }else{//tagline existed in ways.
            hitsCounter++;
            updateWaysCountVec(index,waysCount[index]);
        }
    }*/
   /*
    void updateSetAfterWrite(uint32_t tag,bool write_Allocate){//WRITE BACK
        TagLine currTagLine=TagLine(tag);
        currTagLine.setDirty(true);
        int index=findTagLineIndex(tag);
        int currLRUIndex=getLRU();
        if(index==-1){//not existed in ways which means write miss
            missesCounter++;
            mainMemoryAccessesCounter++;
            if(!write_Allocate){//No_write_Allocate
                return;
            }
            if(ways.size()==numberOfWays){//cache is full
                if(ways[currLRUIndex].getDirty()){
                    mainMemoryAccessesCounter++;
                }
                ways[currLRUIndex]=currTagLine;
                updateWaysCountVec(currLRUIndex,0);
            }else{//cache isnt full
                ways.push_back(currTagLine);
                updateWaysCountVec(ways.size()-1,0);
            }
        }else{//tagline existed in ways.
            hitsCounter++;
            ways[index].setDirty(true);
            updateWaysCountVec(index,waysCount[index]);
        }
    }*/
};
class LevelCache{
private:
    unsigned size;
    unsigned numberOfWays;
    unsigned cyclesNumber;
    int blockSize;
    vector<Set> setsVector;
    unsigned missesCounter=0;
    //unsigned hitsCounter=0;
    //unsigned mainMemoryAccessesCounter=0;
    unsigned instructionsCounter=0;
public:
    //constructors
    LevelCache():size(0),numberOfWays(0),cyclesNumber(0){}
    LevelCache(unsigned size1,unsigned numberOfWays1,unsigned cyclesNumber1
               ,unsigned blockSize1)://everything is 2^X (not cyclesnum)
            size(size1),numberOfWays(numberOfWays1),cyclesNumber(cyclesNumber1),blockSize(blockSize1){
        setsVector=vector<Set>(size/(numberOfWays*blockSize),Set(numberOfWays));
    }
    bool operator==(LevelCache& levelCache) {
        if(size==levelCache.size
        && numberOfWays==levelCache.numberOfWays
        && cyclesNumber==levelCache.cyclesNumber
        && blockSize==levelCache.blockSize
        && missesCounter==levelCache.missesCounter
        && instructionsCounter==levelCache.instructionsCounter
        && setsVector.size()==levelCache.setsVector.size()){
            for(unsigned i=0 ; i<setsVector.size() ; i++){
                bool res=!(setsVector[i]==levelCache.setsVector[i]);
                if(!(setsVector[i]==levelCache.setsVector[i])){
                    std::cout<<"i isss buug"<<i<<std::endl;
                    return false;
                }
            }
            return true;
        }
        return false;
    }


    int calculateSetIndexFromAddress(uint32_t address){
        address=address/blockSize;
        unsigned setsNumber=size/(numberOfWays*blockSize);
        address=address%setsNumber;
        return address;
    }
    int calculateSetIndexFromTag(uint32_t tag){
        unsigned setsNumber=size/(numberOfWays*blockSize);
        tag=tag%setsNumber;
        return tag;
    }
    unsigned findTagLineIndex(uint32_t tag){
        unsigned setIndex=calculateSetIndexFromTag(tag);
        return setsVector[setIndex].findTagLineIndex(tag) ;
    }
    bool tagExist(uint32_t tag){
        //instructionsCounter++;
        unsigned setIndex=calculateSetIndexFromTag(tag);
        if(setsVector[setIndex].findTagLineIndex(tag)==-1){
            //missesCounter++;
            return false;
        }
        //hitsCounter++;
        return true;
    }
    vector<Set>& getSetsVec(){
        return setsVector;
    }
    Set& getSetsVec(unsigned  index){
        return setsVector[index];
    }
    /*
    unsigned getHitsCounter(){
        return hitsCounter;
    }*/
    unsigned getCyclesNumber(){
        return cyclesNumber;
    }
    unsigned getMissesCounter(){
        return missesCounter;
    }
    /*unsigned getMainMemoryAccessesCounter(){
        return mainMemoryAccessesCounter;
    }*/
    unsigned getInstructionsCounter(){
        return instructionsCounter;
    }
    void increaseMissesCounter(){
        missesCounter++;
    }
    void increaseInstructionsCounter(){
        instructionsCounter++;
    }
    unsigned getNumberOfWays(){
        return numberOfWays;
    }
    /*void increaseHitsCounter(){
        hitsCounter++;
    }*/

};
class Cache{
private:
    unsigned memoryCyclesNo;
    unsigned blockSize;
    bool writeAllocate;
    LevelCache l1;
    LevelCache l2;
    unsigned mainMemoryAccessesCounter=0;

public:
    Cache(unsigned memoryCyclesNo1,unsigned blockSize1,int writeAllocate1,unsigned l1Size
          ,unsigned l1Associations,unsigned l1CyclesNo,unsigned l2Size,unsigned l2Associations,unsigned l2CyclesNo){
        memoryCyclesNo=memoryCyclesNo1;
        blockSize=pow(2,blockSize1);
        writeAllocate=(bool)writeAllocate1;
        l1=LevelCache(pow(2,l1Size),pow(2,l1Associations),l1CyclesNo,blockSize);
        l2=LevelCache(pow(2,l2Size),pow(2,l2Associations),l2CyclesNo,blockSize);

    }

    int calculateTagFromAddres(uint32_t address){
        return address/blockSize;
    }

    TagLine getCurrTagLineFromCache(LevelCache& cache,unsigned setIndex,uint32_t tag){
        TagLine currTaglineCache=cache.getSetsVec(setIndex).findTagLine(tag);
        return currTaglineCache;
    }
    void updateCountsLevelCache(LevelCache& cache,unsigned setIndex , uint32_t tag,bool adding=true){//we assume that tag is in cache!
        unsigned currTagIndex=cache.getSetsVec(setIndex).findTagLineIndex(tag);
        //unsigned currTagCount=cache.getSetsVec(setIndex).getWaysCount()[currTagIndex];
        if(adding){
            unsigned currTagCount=cache.getSetsVec(setIndex).getWaysCount()[currTagIndex];

            cache.getSetsVec(setIndex).updateWaysCountVec(currTagIndex,currTagCount);
        }else{//deleting
            vector<int>& waysCountVec=cache.getSetsVec(setIndex).getWaysCount();
            unsigned currTagCount=cache.getSetsVec(setIndex).getWaysCount()[currTagIndex];

            cache.getSetsVec(setIndex).updateWaysCountVecAfterDelete(currTagIndex,currTagCount);//index and its count

    }


}
void setDirtyToTagLineInL2(TagLine currLRUTagLine){
    uint32_t set2Index=l2.calculateSetIndexFromTag(currLRUTagLine.getTag());
    l2.getSetsVec(set2Index).findTagLine(currLRUTagLine.getTag()).setDirty(true);
    updateCountsLevelCache(l2,set2Index,currLRUTagLine.getTag());
}
void deleteFromL1(TagLine currLRUTagLine){
    uint32_t set1Index=l1.calculateSetIndexFromTag(currLRUTagLine.getTag());

    vector<TagLine>& waysVec=l1.getSetsVec(set1Index).getWays();
    unsigned currLRUIndexInL1=l1.getSetsVec(set1Index).findTagLineIndex(currLRUTagLine.getTag());

    updateCountsLevelCache(l1,set1Index,currLRUTagLine.getTag(),false);
    waysVec.erase(waysVec.begin()+currLRUIndexInL1);

}
void addTagLineToCache(LevelCache& cache,unsigned setIndex,TagLine tagline){

    if(cache.getSetsVec(setIndex).getWays().size()==cache.getNumberOfWays()){//cache is full
        TagLine currLRUTagLine=getLRUTagLineFromCache(cache,setIndex);

        bool res1=(l2==cache);
        bool res2=l1.tagExist(currLRUTagLine.getTag());
        if(currLRUTagLine.getDirty() && l1==cache){
            setDirtyToTagLineInL2(currLRUTagLine);

        }else if(l2==cache && l1.tagExist(currLRUTagLine.getTag())){//inclusive!
            deleteFromL1(currLRUTagLine);
        }
        unsigned LRUIndexInCache=cache.findTagLineIndex(currLRUTagLine.getTag());
        cache.getSetsVec(setIndex).getWays()[LRUIndexInCache]=tagline;

    }else{
        cache.getSetsVec(setIndex).getWays().push_back(tagline);
        unsigned currTagIndex=cache.getSetsVec(setIndex).findTagLineIndex(tagline.getTag());
        cache.getSetsVec(setIndex).setWaysCount(currTagIndex,0);
    }
}
unsigned getLRUFromCache(LevelCache& cache,unsigned setIndex){
    return cache.getSetsVec(setIndex).getLRU();
}
TagLine& getLRUTagLineFromCache(LevelCache& cache,unsigned setIndex){
    return cache.getSetsVec(setIndex).getLRUTagLine();
}

void read(uint32_t address){
    uint32_t set1Index=l1.calculateSetIndexFromAddress(address);
    uint32_t set2Index=l2.calculateSetIndexFromAddress(address);
    uint32_t tag=calculateTagFromAddres(address);
    TagLine currTagLine=TagLine(tag);

    l1.increaseInstructionsCounter();
    if(!l1.tagExist(tag)){//doesnt exists in l1.
        l1.increaseMissesCounter();
        l2.increaseInstructionsCounter();
        if(!l2.tagExist(tag)){//doesnt exists in l1 and in l2
            l2.increaseMissesCounter();
            mainMemoryAccessesCounter++;

            addTagLineToCache(l1,set1Index,currTagLine);//the new tagline now is in l1!
            updateCountsLevelCache(l1,set1Index,tag);

            addTagLineToCache(l2,set2Index,currTagLine);//the new tagline now is in l2!
            updateCountsLevelCache(l2,set2Index,tag);

            }else{//exist in l2 and not exist in l1.(also when l1 is full!)
                TagLine currTagLineL2=getCurrTagLineFromCache(l2,set2Index,tag);
                updateCountsLevelCache(l2,set2Index,tag);

                addTagLineToCache(l1,set1Index,currTagLineL2);
                updateCountsLevelCache(l1,set1Index,tag);
            }
        }else{//exist in l1! l1 HIT
            updateCountsLevelCache(l1,set1Index,tag); //VVVV
        }
    }

    void write(uint32_t address){
        uint32_t tag=calculateTagFromAddres(address);
        unsigned set1Index=l1.calculateSetIndexFromAddress(address);
        unsigned set2Index=l2.calculateSetIndexFromAddress(address);
        if(writeAllocate){
            read(address);
            l1.getSetsVec(set1Index).findTagLine(tag).setDirty(true);
        }else{
            l1.increaseInstructionsCounter();
            if(!l1.tagExist(tag)){
                l1.increaseMissesCounter();
                l2.increaseInstructionsCounter();
                if(!l2.tagExist(tag)){
                    l2.increaseMissesCounter();
                    mainMemoryAccessesCounter++;
                }else{
                    l2.getSetsVec(set2Index).findTagLine(tag).setDirty(true);
                    updateCountsLevelCache(l2,set2Index,tag);
                }
            }else{
                l1.getSetsVec(set1Index).findTagLine(tag).setDirty(true);
				updateCountsLevelCache(l1,set1Index,tag);

            }
        }
    }
    /*void updateSetAfterRead(uint32_t tag){//also when tag is in the ways vector
        int index=findTagLineIndex(tag);

        if(index==-1){//not existed in ways which means read miss

            if(ways.size()==numberOfWays){//cache is full
                if(ways[currLRUIndex].getDirty()){
                    mainMemoryAccessesCounter++;
                }
                ways[currLRUIndex]=currTagLine;
                updateWaysCountVec(currLRUIndex,0);
            }else{//cache isnt full
                ways.push_back(currTagLine);
                updateWaysCountVec(ways.size()-1,0);
            }
        }else{//tagline existed in ways.
            hitsCounter++;
            updateWaysCountVec(index,waysCount[index]);
        }
    }*/
    double calculateL1MissRate(){
        //std::cout<<l1.getMissesCounter()<<"  "<<l1.getInstructionsCounter();
        return ((double)l1.getMissesCounter()/(double)l1.getInstructionsCounter());
    }
    double calculateL2MissRate(){
        return ((double)l2.getMissesCounter()/(double)l2.getInstructionsCounter());
    }
    double calculateAvgAccTime(){
        //TODO : REMAH
        double totalCycles=0;
        totalCycles+=l1.getInstructionsCounter()*l1.getCyclesNumber();
        totalCycles+=l2.getInstructionsCounter()*l2.getCyclesNumber();
        totalCycles+=mainMemoryAccessesCounter*memoryCyclesNo;
        return ((double)totalCycles/(double)l1.getInstructionsCounter());
    }

};

int main1(){

    Cache cacheSim=Cache(10,1,0,3,1,2,4,2,5);
    cacheSim.read(0);
    cacheSim.read(0);//doesnt change anything
    cacheSim.read(5);
    cacheSim.read(8);
    cacheSim.read(12);
    cacheSim.read(5);
    cacheSim.read(6);
    cacheSim.read(28);
    return 0;
}

int main(int argc, char **argv) {

	if (argc < 19) {
		cerr << "Not enough arguments" << endl;
		return 0;
	}

	// Get input arguments

	// File
	// Assuming it is the first argument
	char* fileString = argv[1];
	ifstream file(fileString); //input file stream
	string line;
	if (!file || !file.good()) {
		// File doesn't exist or some other error
		cerr << "File not found" << endl;
		return 0;
	}

	unsigned MemCyc = 0, BSize = 0, L1Size = 0, L2Size = 0, L1Assoc = 0,
			L2Assoc = 0, L1Cyc = 0, L2Cyc = 0, WrAlloc = 0;

	for (int i = 2; i < 19; i += 2) {
		string s(argv[i]);
		if (s == "--mem-cyc") {
			MemCyc = atoi(argv[i + 1]);
		} else if (s == "--bsize") {
			BSize = atoi(argv[i + 1]);
		} else if (s == "--l1-size") {
			L1Size = atoi(argv[i + 1]);
		} else if (s == "--l2-size") {
			L2Size = atoi(argv[i + 1]);
		} else if (s == "--l1-cyc") {
			L1Cyc = atoi(argv[i + 1]);
		} else if (s == "--l2-cyc") {
			L2Cyc = atoi(argv[i + 1]);
		} else if (s == "--l1-assoc") {
			L1Assoc = atoi(argv[i + 1]);
		} else if (s == "--l2-assoc") {
			L2Assoc = atoi(argv[i + 1]);
		} else if (s == "--wr-alloc") {
			WrAlloc = atoi(argv[i + 1]);
		} else {
			cerr << "Error in arguments" << endl;
			return 0;
		}
	}
    Cache cache=Cache(MemCyc,BSize,WrAlloc,L1Size, L1Assoc, L1Cyc ,L2Size, L2Assoc, L2Cyc);

	while (getline(file, line)) {

		stringstream ss(line);
		string address;
		char operation = 0; // read (R) or write (W)
		if (!(ss >> operation >> address)) {
			// Operation appears in an Invalid format
			cout << "Command Format error" << endl;
			return 0;
		}

		// DEBUG - remove this line
		//cout << "operation: " << operation;

		string cutAddress = address.substr(2); // Removing the "0x" part of the address

		// DEBUG - remove this line
		//cout << ", address (hex)" << cutAddress;

		unsigned long int num = 0;
		num = strtoul(cutAddress.c_str(), NULL, 16);

		// DEBUG - remove this line
		//cout << " (dec) " << num << endl;

		if(operation=='r') cache.read(num);
		else if(operation=='w')cache.write(num);

	}

	double L1MissRate=cache.calculateL1MissRate();
	double L2MissRate=cache.calculateL2MissRate();
	double avgAccTime=cache.calculateAvgAccTime();


	printf("L1miss=%.03f ", L1MissRate);
	printf("L2miss=%.03f ", L2MissRate);
	printf("AccTimeAvg=%.03f\n", avgAccTime);

	return 0;
}
