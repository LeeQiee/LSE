/*!
* @file 	Rotations.hpp
* @author 	Michael Blösch
* @date		10.10.2012
* @brief	Rotation stuff... convention:
* 			roll-pitch-yaw: 	alias
* 			rotation matrix: 	alibi
* 			rotation vector: 	alibi
* 			quaternion:			alibi
 */

#ifndef LSE_ROTATION_HPP_
#define LSE_ROTATION_HPP_

#include <Eigen/Dense>

namespace LSE {
namespace Rotations {

typedef Eigen::Vector4d Quat;

/*! Converts vector to sqew matrix
 * @return	corresponding sqew-matrix
 * @param[in] 	v	vector
 */
inline Eigen::Matrix3d vecToSqew(const Eigen::Vector3d& v){
	Eigen::Matrix3d M;
	M << 0, -v(2), v(1), v(2), 0, -v(0), -v(1), v(0), 0;
	return M;
}

/*! Limits the norm of a rotation vector to pi
 * @return 	rescaled rotation vector
 * @param[in] 	v	rotation vector
 */
inline Eigen::Vector3d rangePi(const Eigen::Vector3d& v){
	const double a = v.norm();
	if (a<=M_PI){
		return v;
	} else {
		const double a2 = -2.0*M_PI*floor((a+M_PI)/(2*M_PI))+a;
		return v/a*a2;
	}
}

/*! Converts a quaternion to a rotation matrix
 * @return 	corresponding rotation matrix
 * @param[in]	q	quaternion
 */
inline Eigen::Matrix3d quatToRotMat(const Quat& q){
	Eigen::Vector3d v = q.block(0,0,3,1);
	return (2*q(3)*q(3)-1)*Eigen::Matrix3d::Identity() + 2*q(3)*vecToSqew(v) + 2*v*v.transpose();
}

/*! Converts a quaternion to a rotation vector
 * @return 	corresponding rotation vector
 * @param[in]	q	quaternion
 */
inline Eigen::Vector3d quatToRotVec(const Quat& q){
	Eigen::Vector3d v;
	const double c = q(3);
	v = q.block(0,0,3,1);
	const double s = v.norm();
	if(s >= 1e-10){
		const double a = 2*atan2(s,c);
		return v*a/s;
	} else {
		return v*2;
	}
}

/*! Converts a rotation vector to a quaternion
 * @return 	corresponding quaternion
 * @param[in]	v	rotation vector
 */
inline Quat rotVecToQuat(const Eigen::Vector3d& v){
	Quat q;
	const double a = v.norm();
	q(3) = cos(a/2);
	if(a >= 1e-10){
		q.block(0,0,3,1) = sin(a/2)/a*v;
	} else {
		q.block(0,0,3,1) = v;
	}
	q.normalize();
	return q;
}

/*! Computes the inverse of a quaternion
 * @return 	corresponding quaternion inverse
 * @param[in]	q	quaternion
 */
inline Quat quatInverse(const Quat& q){
	Quat q2;
	q2.block(0,0,3,1) = -q.block(0,0,3,1);
	q2(3) = q(3);
	return q2;
}

/*! Returns the identity quaternion
 * @return 	identity quaternion
 */
inline Quat quatIdentity(){
	Quat q;
	q.setZero();
	q(3) = 1;
	return q;
}

/*! Computes the left-hand multiplication matrix from a given quaternion
 * @return 	left-hand multiplication matrix
 * @param[in]	q	quaternion
 */
inline Eigen::Matrix<double,4,4> quatL(const Quat& q){
	Eigen::Matrix<double,4,4> M;
	M.setIdentity();
	M = M*q(3);
	M.block(0,0,3,3) += vecToSqew(q.block(0,0,3,1));
	M.block(3,0,1,4) = -q.transpose();
	M.block(0,3,4,1) = q;
	return M;
}

/*! Computes the right-hand multiplication matrix from a given quaternion
 * @return 	right-hand multiplication matrix
 * @param[in]	q	quaternion
 */
inline Eigen::Matrix<double,4,4> quatR(const Quat& q){
	Eigen::Matrix<double,4,4> M;
	M.setIdentity();
	M = M*q(3);
	M.block(0,0,3,3) -= vecToSqew(q.block(0,0,3,1));
	M.block(3,0,1,4) = -q.transpose();
	M.block(0,3,4,1) = q;
	return M;
}

inline Eigen::Vector3d quatToYpr(const Quat& q){
    Eigen::Vector3d ypr;
    ypr(0) = atan2(2*(-q(3)*q(0)+q(1)*q(2)),1-2*(pow(q(0),2)+pow(q(1),2)));
    ypr(1) = asin(2*(-q(3)*q(1)-q(0)*q(2)));
    ypr(2) = atan2(2*(-q(3)*q(2)+q(0)*q(1)),1-2*(pow(q(1),2)+pow(q(2),2)));
    return ypr;
}

inline Quat yprToQuat(const Eigen::Vector3d& v){
	Quat q;
	const double c_phi = cos(v(0)/2);
	const double s_phi = sin(v(0)/2);
	const double c_theta = cos(v(1)/2);
	const double s_theta = sin(v(1)/2);
	const double c_psi = cos(v(2)/2);
	const double s_psi = sin(v(2)/2);
    q(0) = c_phi*s_theta*s_psi-c_theta*c_psi*s_phi;
    q(1) = -c_phi*s_theta*c_psi-c_theta*s_psi*s_phi;
    q(2) = -c_phi*c_theta*s_psi+s_theta*c_psi*s_phi;
    q(3) = c_phi*c_theta*c_psi+s_theta*s_psi*s_phi;
    return q;
}

inline Eigen::Vector3d quatToRpy(const Quat& q){
    Eigen::Vector3d rpy;
    rpy(0) = atan2(2*(-q(2)*q(1)-q(3)*q(0)), pow(q(2),2) + pow(q(3),2) - pow(q(0),2)- pow(q(1),2));
    rpy(1) = asin(2*(q(0)*q(2)-q(3)*q(1)));
    rpy(2) = atan2(-2*q(0)*q(1) - 2*q(3)*q(2), pow(q(0),2) + pow(q(3),2) - pow(q(2),2)- pow(q(1),2));
    return rpy;
}

inline Quat rpyToQuat(const Eigen::Vector3d& v){
	Quat q;
	const double c_phi = cos(v(0)/2);
	const double s_phi = sin(v(0)/2);
	const double c_theta = cos(v(1)/2);
	const double s_theta = sin(v(1)/2);
	const double c_psi = cos(v(2)/2);
	const double s_psi = sin(v(2)/2);
    q(0) = -c_phi*c_theta*s_psi-s_theta*c_psi*s_phi;
    q(1) = -c_phi*s_theta*c_psi+c_theta*s_psi*s_phi;
    q(2) = -c_phi*s_theta*s_psi-c_theta*c_psi*s_phi;
    q(3) = c_phi*c_theta*c_psi-s_theta*s_psi*s_phi;
    return q;
}

inline Eigen::Matrix3d rpyToEar(const Eigen::Vector3d& rpy){
	Eigen::Matrix3d M;
	M.setZero();
	const double cp = cos(rpy(1));
	const double sp = sin(rpy(1));
	const double cy = cos(rpy(2));
	const double sy = sin(rpy(2));
	M(0,0) = cp*cy;
	M(0,1) = sy;
	M(0,2) = 0;
	M(1,0) = -cp*sy;
	M(1,1) = cy;
	M(1,2) = 0;
	M(2,0) = sp;
	M(2,1) = 0;
	M(2,2) = 1;
	M << cp*cy, sy, 0, -cp*sy, cy, 0, sp, 0, 1;
	return M;
}

inline Eigen::Matrix3d rpyToEarInv(const Eigen::Vector3d& rpy){
	Eigen::Matrix3d M;
	M.setZero();
	const double cp = cos(rpy(1));
	if(cp>1e-10){
		const double cpi = 1.0/cp;
		const double tp = tan(rpy(1));
		const double cy = cos(rpy(2));
		const double sy = sin(rpy(2));
		M(0,0) = cpi*cy;
		M(0,1) = -cpi*sy;
		M(0,2) = 0;
		M(1,0) = sy;
		M(1,1) = cy;
		M(1,2) = 0;
		M(2,0) = -cy*tp;
		M(2,1) = sy*tp;
		M(2,2) = 1;
	}
	return M;
}

//static void rpyToQuat(const Eigen::Vector3d& rpy, Eigen::Quaterniond& q){
//	const double cy = cos(rpy(2)/2);
//	const double sy = sin(rpy(2)/2);
//	const double cc = cos(rpy(1)/2)*cos(rpy(0)/2);
//	const double cs = cos(rpy(1)/2)*sin(rpy(0)/2);
//	const double sc = sin(rpy(1)/2)*cos(rpy(0)/2);
//	const double ss = sin(rpy(1)/2)*sin(rpy(0)/2);
//
//	q.w() = cy*cc-sy*ss;
//	q.x() = -cy*cs-sy*sc;
//	q.y() = -cy*sc+sy*cs;
//	q.z() = -cy*ss-sy*cc;
//	q.normalize();
//}
//


class NQuat{
public:
	/*! Element access operator overloading (const version) */
	const double& operator()(unsigned int i) const{ return q_[i];}
	/*! Element access operator overloading */
	double& operator()(unsigned int i) { return q_[i];}

	inline void normalize(){
		double a = std::sqrt(q_[0]*q_[0]+q_[1]*q_[1]+q_[2]*q_[2]+q_[3]*q_[3]);
		if(a>1e-10){
			q_[0] = q_[0]/a;
			q_[1] = q_[1]/a;
			q_[2] = q_[2]/a;
			q_[3] = q_[3]/a;
		} else {
			q_[0] = 0;
			q_[1] = 0;
			q_[2] = 0;
			q_[3] = 1;
		}
	}

	NQuat operator* (const NQuat& q){
		NQuat temp;
		temp(0) = q(0) - q_[2]*q(1) + q_[1]*q(2) + q_[0]*q(3);
		temp(1) = q_[2]*q(0) + q(1) - q_[0]*q(2) + q_[1]*q(3);
		temp(2) = -q_[1]*q(0) + q_[0]*q(1) + q(2) + q_[2]*q(3);
		temp(3) = -q_[0]*q(0) - q_[1]*q(1) - q_[2]*q(2) + q_[3]*q(3);
		return temp;
    }

	void setIdentity(){
		q_[0] = 0;
		q_[1] = 0;
		q_[2] = 0;
		q_[3] = 1;
	}

private:
	double q_[4];

};

//Eigen::Matrix<double,4,4> M;
//M.setIdentity();
//M = M*q(3);
//M.block(0,0,3,3) += vecToSqew(q.block(0,0,3,1));
//M.block(3,0,1,4) = -q.transpose();
//M.block(0,3,4,1) = q;
//return M;

//inline Eigen::Matrix3d vecToSqew(const Eigen::Vector3d& v){
//	Eigen::Matrix3d M;
//	M << 0, -v(2), v(1), v(2), 0, -v(0), -v(1), v(0), 0;
//	return M;
//}

inline Eigen::Vector3d NquatToRotVec(const NQuat& q){
	Eigen::Vector3d v;
	const double c = q(3);
	v(0) = q(0);
	v(1) = q(1);
	v(2) = q(2);
	const double s = v.norm();
	if(s >= 1e-10){
		const double a = 2*atan2(s,c);
		return v*a/s;
	} else {
		return v*2;
	}
}

inline NQuat NrotVecToQuat(const Eigen::Vector3d& v){
	NQuat q;
	const double a = v.norm();
	q(3) = cos(a/2);
	if(a >= 1e-10){
		const double b = sin(a/2)/a;
		q(0) = b*v(0);
		q(1) = b*v(1);
		q(2) = b*v(2);
	} else {
		q(0) = v(0);
		q(1) = v(1);
		q(2) = v(2);
	}
	q.normalize();
	return q;
}

template<int N, int M, int L>
class LieG{
public:
	LieG(){
	}

	typedef Eigen::Matrix<double,N+M*3+L*3,1> LieA;

	void reset(){
		for(int i=0;i<N;i++){
			scalars_[i] = 0;
		}
		for(int i=0;i<M;i++){
			vectors_[i].setZero();
		}
		for(int i=0;i<L;i++){
			quats_[i].setIdentity();
		}
	}

	int getDim(){
		return N+3*(M+L);
	}

	/*! Element access operator overloading (const version) */
	const Eigen::Vector3d& operator[](unsigned int i) const{ return vectors_[i];}
	/*! Element access operator overloading */
	Eigen::Vector3d& operator[](unsigned int i) { return vectors_[i];}

	/*! Element access operator overloading (const version) */
	const NQuat& operator()(unsigned int i) const{ return quats_[i];}
	/*! Element access operator overloading */
	NQuat& operator()(unsigned int i) { return quats_[i];}

	template<int NN, int MM, int LL>
    LieA operator- (const LieG<NN,MM,LL>& y){
		LieA temp;
		for(int i=0;i<NN;i++){
			temp(i) = scalars_[i]-y.scalars_[i];
		}
		for(int i=0;i<MM;i++){
			temp.block<3,1>(NN+i*3,0) = vectors_[i]-y[i];
		}
		for(int i=0;i<LL;i++){
			temp.block<3,1>(NN+MM*3+i*3,0) = NquatToRotVec(quats_[i]*quatInverse(y(i)));
		}
		return temp;
    }

	template<int NN, int MM, int LL>
    LieG<NN,MM,LL> operator+ (LieA& d){
		LieG<NN,MM,LL> temp;
		for(int i=0;i<NN;i++){
			temp.scalars_[i] = scalars_[i]+d(i);
		}
		for(int i=0;i<MM;i++){
			temp[i] = vectors_[i]+d.block<3,1>(NN+i*3,0);
		}
		for(int i=0;i<LL;i++){
			temp(i) = NrotVecToQuat(d.block<3,1>(NN+MM*3+i*3,0))*quats_[i];
		}
		return temp;
    }

	double scalars_[N];
	Eigen::Vector3d vectors_[M];
	NQuat quats_[L];
};

}
}

#endif /* LSE_ROTATION_HPP_ */
