#include"photonmap.h"
#include<iostream>
#include<cmath>

Nearestphotons::Nearestphotons() {
	max_photons = found = 0;
	got_heap = false;
	dist2 = NULL;
	photons = NULL;
}

void Photonmap::BalanceSegment( Photon* porg , int index , int st , int en ) {
	if ( st == en ) {//1）终止条件 
		photons[index] = porg[st];//创建叶子
		return;
	}

	int med = 1;//2）得到 光子个数的中间值 
	while ( 4 * med <= en - st + 1 ) med <<= 1;

	if ( 3 * med <= en - st + 1 ) med = med * 2 + st - 1;
		else med = en + 1 - med;
	
	int axis = 2;//3）得到切分轴(或切分平面)，0=x，1=y，2=Z ---包围所有光子的两个角点（最大、最小），x、y、z三个轴向上差距最大的那个轴 
	if ( box_max.x - box_min.x > box_max.y - box_min.y && box_max.x - box_min.x > box_max.z - box_min.z ) axis = 0; else
	if ( box_max.y - box_min.y > box_max.z - box_min.z ) axis = 1;

	MedianSplit( porg , st , en , med , axis );//得到切分中间值 
	photons[index] = porg[med];  
	photons[index].plane = axis;
// 将数据集分为两个子集
	if ( st < med ) {
		double tmp = box_max.GetCoord( axis );
		box_max.GetCoord( axis ) = photons[index].pos.GetCoord( axis );
		BalanceSegment( porg , index * 2 , st , med - 1 ); //子递归构造左子树  集递归调用buildKdTree函数  
		box_max.GetCoord( axis ) = tmp;
	}

	if ( med < en ) {
		double tmp = box_min.GetCoord( axis );
		box_min.GetCoord( axis ) = photons[index].pos.GetCoord( axis );
		BalanceSegment( porg , index * 2 + 1 , med + 1 , en ); //递归构造右子树  子集递归调用buildKdTree函数  
		box_min.GetCoord( axis ) = tmp;
	}
}

void Photonmap::MedianSplit( Photon* porg , int st , int en , int med , int axis ) {
	int l = st , r = en;

	while ( l < r ) {
		double key = porg[r].pos.GetCoord( axis );//以末尾元素为候选点 
		int i = l - 1 , j = r;
		for ( ; ; ) {
			while ( porg[++i].pos.GetCoord( axis ) < key );//从左边向中间扫描，得到较大的点 
			while ( porg[--j].pos.GetCoord( axis ) > key && j > l );//从右边向中间扫描 ，得到较小的点 
			if ( i >= j ) break;
			std::swap( porg[i] , porg[j] );
		}

		std::swap( porg[i] , porg[r] );
		if ( i >= med ) r = i - 1;
		if ( i <= med ) l = i + 1;
	}
}
/*
int Partion(int a[], int l, int r, int p)  {//进行划分过程
    swap(a[p], a[l]);  
    int i = l;  
    int j = r;  
    int pivot = a[l];  
    while(i < j)  {
        while( in_a[a[j] ]>= in_a[ pivot] && i < j)  
            j--;  
        a[i] = a[j];  
        while( in_a[a[i] ]<=in_a[ pivot] && i < j)  
            i++;  
        a[j] = a[i];  
    }  
    a[i] = pivot;  
    return i;  
} */
void Photonmap::LocatePhotons( Nearestphotons* np , int index ) {
//	Kd树的查询：判断是否叶子节点，是则将叶子节点的温度加入累加和、累加个数增1；判断左子树完全在所查询区域内，则该节点的温度加入累加和、累加个数。判断左子树与所查询区域相交且不空，则递归查询左子树；判断右子树完全在所查询区域内，则该节点的温度加入累加和、累加个数。判断右子树与所查询区域相交且不空，则递归查询右子树
	if ( index > stored_photons ) return;
	Photon *photon = &photons[index];

	if ( index * 2 <= stored_photons ) {
		double dist = np->pos.GetCoord( photon->plane ) - photon->pos.GetCoord( photon->plane );
		if ( dist < 0 ) {
			LocatePhotons( np , index * 2 );
			if ( dist * dist < np->dist2[0] ) LocatePhotons( np , index * 2 + 1 );
		} else {
			LocatePhotons( np , index * 2 + 1 );
			if ( dist * dist < np->dist2[0] ) LocatePhotons( np , index * 2 );
		}
	}

	double dist2 = photon->pos.Distance2( np->pos );
	if ( dist2 > np->dist2[0] ) return;

	if ( np->found < np->max_photons ) {
		np->found++;
		np->dist2[np->found] = dist2;
		np->photons[np->found] = photon;
	} else {
		if ( np->got_heap == false ) {
			for ( int i = np->found >> 1 ; i >= 1 ; i-- ) {
				int par = i;
				Photon* tmp_photon = np->photons[i];
				double tmp_dist2 = np->dist2[i];
				while ( ( par << 1 ) <= np->found ) {
					int j = par << 1;
					if ( j + 1 <= np->found && np->dist2[j] < np->dist2[j + 1] ) j++;
					if ( tmp_dist2 >= np->dist2[j] ) break;
					
					np->photons[par] = np->photons[j];
					np->dist2[par] = np->dist2[j];
					par = j;
				}
				np->photons[par] = tmp_photon;
				np->dist2[par] = tmp_dist2;
			}
			np->got_heap = true;
		}

		int par = 1;
		while ( ( par << 1 ) <= np->found ) {
			int j = par << 1;
			if ( j + 1 <= np->found && np->dist2[j] < np->dist2[j + 1] ) j++;
			if ( dist2 > np->dist2[j] ) break;

			np->photons[par] = np->photons[j];
			np->dist2[par] = np->dist2[j];
			par = j;
		}
		np->photons[par] = photon;
		np->dist2[par] = dist2;

		np->dist2[0] = np->dist2[1];
	}
}

Color Photonmap::GetIrradiance( Collider* collider , double max_dist , int n ) {
	Color ret;

	Nearestphotons np;
	np.pos = collider->C;
	np.max_photons = n;
	np.dist2 = new double[n + 1];
	np.photons = new Photon*[n + 1];
	np.dist2[0] = max_dist * max_dist;

	LocatePhotons( &np , 1 );
	if ( np.found <= 8 ) return ret;

	for ( int i = 1 ; i <= np.found ; i++ ) {
		Vector3 dir = np.photons[i]->dir;
		if ( collider->N.Dot( dir ) < 0 )
			ret += np.photons[i]->power * collider->GetPrimitive()->GetMaterial()->BRDF(-dir, collider->N, -collider->I);
	}
	
	ret = ret * (4 / (emit_photons * np.dist2[0]));
	return ret;
}

double Photonmap::GetRadius2(Collider* collider, double max_dist, int n) {
	Nearestphotons np;
	np.pos = collider->C;
	np.max_photons = n;
	np.dist2 = new double[n + 1];
	np.photons = new Photon*[n + 1];
	np.dist2[0] = max_dist * max_dist;

	LocatePhotons( &np , 1 );
	return np.dist2[0];
}

void Photonmap::Store( Photon photon ) {
	if ( stored_photons >= max_photons ) return;
	photons[++stored_photons] = photon;
	box_min = Vector3( std::min( box_min.x , photon.pos.x ) , std::min( box_min.y , photon.pos.y ) , std::min( box_min.z , photon.pos.z ) );
	box_max = Vector3( std::max( box_max.x , photon.pos.x ) , std::max( box_max.y , photon.pos.y ) , std::max( box_max.z , photon.pos.z ) );
}

void Photonmap::Balance() {
	Photon* porg = new Photon[stored_photons + 1];

	for ( int i = 0 ; i <= stored_photons ; ++i )
		porg[i] = photons[i];
	
	BalanceSegment( porg , 1 , 1 , stored_photons );
	delete[] porg;
}
