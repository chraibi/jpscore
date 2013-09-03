/*
 * mesh.cpp
 *
 *  Created on: 21.08.2013
 *      Author: dominik
 */

#include "mesh.h"
#include <iostream>

MeshEdge::MeshEdge(int n1,int n2,int c1, int c2,Point p1,Point p2):NavLine(Line(p1,p2)){
	_n1=n1;
	_n2=n2;
	_c1=c1;
	_c2=c2;
}

MeshCell::MeshCell(double midx,double midy,std::vector<int> node_id,
			 double *normvec,std::vector<int> edge_id,
			 std::vector<int> wall_id,int id){
	//_midx=midx;
	//_midy=midy;
	_mid=Point(midx,midy);
	_node_id=node_id;
	for(int i=0;i<3;i++)
		_normvec[i]=normvec[i];
	_edge_id=edge_id;
	_wall_id=wall_id;
	_tc_id=id;
}

MeshCell::~MeshCell(){
	//delete[] _normvec;
}

MeshCellGroup::MeshCellGroup(std::string groupname,std::vector<MeshCell*> cells){
	_groupname=groupname;
	_cells=cells;
}
MeshCellGroup::~MeshCellGroup(){
	for(unsigned int i=0;i<_cells.size();i++)
			delete _cells[i];
}
std::vector<MeshCell*> MeshCellGroup::get_cells(){
	return _cells;
}

MeshData::MeshData(){
	_mNodes=std::vector<Point*>();
	_mEdges=std::vector<MeshEdge*>();
	_mOutEdges=std::vector<MeshEdge*>();
	_mCellGroups=std::vector<MeshCellGroup*>();
	_mCellCount=0;
}
MeshData::~MeshData(){
	std::cout<<"\tStart Destructor Meshdata"<<std::endl;
	for(unsigned int i=0;i<_mNodes.size();i++)
		delete _mNodes[i];
	for(unsigned int i=0;i<_mEdges.size();i++)
			delete _mEdges[i];
	for(unsigned int i=0;i<_mOutEdges.size();i++)
			delete _mOutEdges[i];
	for(unsigned int i=0;i<_mCellGroups.size();i++)
			delete _mCellGroups[i];
	std::cout<<"\tEnd Destructor Meshdata"<<std::endl;
}

unsigned int calc_CellCount(std::vector<MeshCellGroup*> mcg){
	unsigned int count=0;
	for (unsigned int i=0;i<mcg.size();i++)
		count+=mcg.at(i)->get_cells().size();

	return count;
}

MeshData::MeshData(std::vector<Point*> mn,std::vector<MeshEdge*> me,
		std::vector<MeshEdge*> moe,std::vector<MeshCellGroup*> mcg){
	_mNodes=mn;
	_mEdges=me;
	_mOutEdges=moe;
	_mCellGroups=mcg;
	_mCellCount=calc_CellCount(mcg);
}

MeshCell* MeshData::getCellAtPos(unsigned int tpos){
	if( tpos<0 || tpos>= this->get_cellCount())
		return NULL;
	else{
		for(unsigned int i=0;i<_mCellGroups.size();i++){
			if(tpos<_mCellGroups.at(i)->get_cells().size()){
				return _mCellGroups.at(i)->get_cells().at(tpos);
			}
			else{
				tpos-=_mCellGroups.at(i)->get_cells().size();
			}
		}

		return NULL;
	}
}

 MeshCell* MeshData::findCell(Point test, int& cell_id){

	int tmp_id=-1;
	std::vector<MeshCellGroup*>::const_iterator it_g;
	for(it_g=_mCellGroups.begin();it_g!=_mCellGroups.end();it_g++){
		std::vector<MeshCell*>::const_iterator it_c;
		std::vector<MeshCell*> act_cg=(*it_g)->get_cells();
		for(it_c=act_cg.begin();it_c!=act_cg.end();it_c++){
			bool found=true;
			std::vector<int> act_n=(*it_c)->get_nodes();
			int count_nodes=act_n.size();
			double n1x= _mNodes.at(act_n.at(0))->GetX();
			double n1y= _mNodes.at(act_n.at(0))->GetY();

			for(int pos=0;pos<count_nodes;pos++){
				double n2x= _mNodes.at(act_n.at((pos+1)%count_nodes))->GetX();
				double n2y= _mNodes.at(act_n.at((pos+1)%count_nodes))->GetY();

				Point temp_nxny(n2y-n1y,n1x-n2x);
				Point temp_xy=test-Point(n1x,n1y);

				if (temp_xy.ScalarP(temp_nxny)>0){
					found=false;
					break;
				}
				n1x=n2x;
				n1y=n2y;
			}
			tmp_id++;
			if (found){
				cell_id=tmp_id;
				return (*it_c);
			}

		}
	}
	// Point test is in no polygon of MeshData
	cell_id=-1;
	return NULL;
}
/*
std::istream& operator>>(std::istream& is, MeshNode& mn){
	is>>mn._x>>mn._y;
	return is;
}
*/
