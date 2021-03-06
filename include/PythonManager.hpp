/*!
* @file 	PythonManager.hpp
* @author 	Michael Blösch
* @date		10.10.2012
* @brief	Wrapper Class
 */

#ifndef LSE_PYTHONMANAGER_HPP_
#define LSE_PYTHONMANAGER_HPP_

#ifdef WRAP_PYTHON
#include <Python.h>
#include <boost/python.hpp>
#include <numpy/arrayobject.h>
#endif

#include "Manager.hpp"

namespace LSE {

/*! PythonManager */
class PythonManager{
public:
	/* -------------------- Constructor/Destructor --------------------- */
	/*! Constructor
	 * @param[in]	pFilename	filename of parameter-file
	 * @param[in] 	f			function pointer to forward kinematic
	 * @param[in] 	J			function pointer to forward kinematic Jacobian (default = NULL)
	 */
	PythonManager(std::string filename);
	/*! Destructor */
	virtual ~PythonManager();

//	/* -------------------- Measurement Handling --------------------- */
//	/*! Adds an IMU measurement
//	 * @param[in]	t	time of measurement
//	 * @param[in] 	m	measurement data
//	 */
//	void addImuMeas(const double& t,const ImuMeas& m);
//	/*! Adds encoders measurements
//	 * @param[in]	t	time of measurement
//	 * @param[in] 	m	measurement data
//	 */
//	void addEncMeas(const double& t,const EncMeas& m);
//	/*! Adds pose sensor measurements
//	 * @param[in]	t	time of measurement
//	 * @param[in] 	m	measurement data
//	 */
//	void addPosMeas(const double& t,const PosMeas& m);
//	/*! Searches for the first measurement after time t
//	 * @return const pointer to measurement
//	 * @param[in/out]	t	time of measurement, changed to precise measurement time
//	 */
//	const ImuMeas* getImuMeas(double& t);
//	/*! Searches for the first measurement after time t
//	 * @return const pointer to measurement
//	 * @param[in/out]	t	time of measurement, changed to precise measurement time
//	 */
//	const EncMeas* getEncMeas(double& t);
//	/*! Searches for the first measurement after time t
//	 * @return const pointer to measurement
//	 * @param[in/out]	t	time of measurement, changed to precise measurement time
//	 */
//	const PosMeas* getPosMeas(double& t);
//
//	/* -------------------- Filter and Calibration Handling --------------------- */
//	/*! Updates the filter to time t
//	 * @param[in]	t	desired update time
//	 */
//	void update(const double& t);
//	/*! Updates the filter to the newest measurement time */
//	void update();
//	/*! Return current estimate of robot state (main body)
//	 * @return	current robot state
//	 */
//	State getEst();
//	/*! Resets the filter
//	 * @param[in]	t	time used to initialize new state estimate
//	 */
//	void resetEstimate(const double& t);
//	/*! Identifies the delay between the sensor modalities
//	 * @return	results of identification (0:fail, 1:success)
//	 * @param[in]	t	end of identification interval
//	 * @param[in]	T	length of identification interval
//	 */
//	int delayIdentification(const double& t,const double& T);
//
//	/* -------------------- Time delay handling of modalities --------------------- */
#ifdef WRAP_PYTHON
	void addImuMeas_python(double t, PyObject* pyf, PyObject* pyw);
	//	M = PyArray_SimpleNew(2, dims, NPY_DOUBLE);
	int getImuMeas_python(double t, PyObject* pyf, PyObject* pyw);
	void addEncMeas_python(double t, PyObject* pye, PyObject* pyv, PyObject* pyCF);
	int getEncMeas_python(double t, PyObject* pye, PyObject* pyv, PyObject* pyCF);
	void addPosMeas_python(double t, PyObject* pyr, PyObject* pyq);
	int getPosMeas_python(double t, PyObject* pyr, PyObject* pyq);
	void addOflMeas_python(double t, PyObject* pyx, PyObject* pyu);
	void clearMeas_python();
	void update_pythont(double t);
	void update_python();
	void getEst_python(PyObject* pyx);
	void getSlippage_python(PyObject* pyx);
	void resetEstimate_python(double t);
	int delayIdentification_python(double t,double T);
	void setImuTD_python(double TD);
	void setEncTD_python(double TD);
	void setPosTD_python(double TD);
	double getImuTD_python();
	double getEncTD_python();
	double getPosTD_python();

	PyObject* quatL_python(PyObject* quat);
	PyObject* quatR_python(PyObject* quat);
	PyObject* quatToYpr_python(PyObject* quat);
	PyObject* yprToQuat_python(PyObject* quat);
	PyObject* quatToRpy_python(PyObject* quat);
	PyObject* rpyToQuat_python(PyObject* quat);
	PyObject* quatToRotVec_python(PyObject* quat);
	PyObject* rotVecToQuat_python(PyObject* quat);
	template<int N,int M>
	void EigenToPyObject(const Eigen::Matrix<double,N,M> &EigM,const PyObject* PyM);
	template<int N,int M>
	void PyObjectToEigen(const PyObject* const PyM, Eigen::Matrix<double,N,M> &EigM);


#if USE_CERES
	int robotCalibration_python(double t,double T);
	int getLengthOfBC_python();
	void getBCData_python(PyObject* X);
#endif
#endif

private:
	Manager* pManager_;
};

}

#endif /* LSE_PYTHONMANAGER_HPP_ */
