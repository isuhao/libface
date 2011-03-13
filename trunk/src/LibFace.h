/** ===========================================================
 * @file
 *
 * This file is a part of libface project
 * <a href="http://libface.sourceforge.net">http://libface.sourceforge.net</a>
 *
 * @date    2010-02-18
 * @brief   Lead Face library class.
 * @section DESCRIPTION
 *
 * @author Copyright (C) 2010 by Alex Jironkin
 *         <a href="alexjironkin at gmail dot com">alexjironkin at gmail dot com</a>
 * @author Copyright (C) 2010 by Aditya Bhatt
 *         <a href="adityabhatt at gmail dot com">adityabhatt at gmail dot com</a>
 * @author Copyright (C) 2010 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2010 by Marcel Wiesweg
 *         <a href="mailto:marcel dot wiesweg at gmx dot de">marcel dot wiesweg at gmx dot de</a>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef _LIBFACE_H_
#define _LIBFACE_H_

#include "Eigenfaces.h"
//#include "HMMfaces.h"
#include "LibFaceConfig.h"
#include "LibFaceCore.h"

namespace libface {

/** This defines that everything is initialised in libface, both detection and recognition.
 */
enum Mode {
	ALL,
	DETECT,
	EIGEN,
	HMM
};

class FACEAPI LibFace {

public:

	/**
	 * Constructor for the LibFace class. Type argument specifies what you want to do with the library.
	 * By default ALL is specified, meaning that it will do both face detection and recognition.
	 * DETECT can be specified to do only face detection.
	 * EIGEN can be specified to do only face recognition, without face detection.
	 *
	 * @param type Type of the face recognition/detection/both to use. ALL by default.
	 * @param configDir Config directory of the libface library. If there is libface.xml library will try to load it.
	 * By default it is empty ("").
	 * @param cascadeDir Directory where haar cascade is. By default it is /usr/share/opencv/haarcascades
	 */

#ifdef WIN32
	LibFace(Mode type = ALL, const std::string& configDir = ".", const std::string& cascadeDir = std::string(OPENCVDIR)+"/data/haarcascades");
#else
	LibFace(Mode type = ALL, const std::string& configDir = ".", const std::string& cascadeDir = std::string(OPENCVDIR)+"/haarcascades");
#endif

	/**
	 * Deconstructor for the library.
	 */
	~LibFace();

	// OpenCV compatibility methods

	/**
	 * Get the count of all faces trained
	 */
	int count() const;


	/**
	 * Method for detecting faces in the picture with the pointer. It will attempt to
	 * check if this file was last to be loaded, if it has been then it will use last pointer. The IDs for
	 * all faces will be -1.
	 *
	 * @param img A pointer to the IplImage for face detection.
	 *
	 * @return Vector of Face objects with ID set to -1 on each.
	 */
	std::vector<Face*>* detectFaces(const IplImage* img);
	// API-agnostic methods

	/**
	 * Method for detecting faces in the picture with the specified filename. It will attempt to
	 * check if this file was last to be loaded, if it has been then it will use last pointer. The IDs for
	 * all faces will be -1.
	 *
	 * @param filename Filename of the file to find faces in.
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster.
	 * Default not scaled (1). NOT USED at the moment.
	 *
	 * @return Vector of Face objects with ID set to -1 on each.
	 */
	std::vector<Face*>* detectFaces(const std::string& filename, int scaleFactor=1);

	/**
	 * Method for detecting faces in the picture with the specified encoding. The IDs for
	 * all faces will be -1.
	 *
	 * @param image A pointer to the char encoding.
	 * @param width Image width.
	 * @param height Image height.
	 * @param step Step.
	 * @param depth Image depth (default: IPL_DEPTH_8U).
	 * @param channels Number of channels in the image (default: 1).
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster.
	 * Default not scaled (1). NOT USED at the moment.
	 *
	 * @return Vector of Face objects with ID set to -1 on each.
	 */
	std::vector<Face*>* detectFaces(const char* image, int width , int height , int step , int depth = IPL_DEPTH_8U, int channels = 1, int scaleFactor=1 );



	/**
	 *  Method for getting the configuration from the face recognition. There is no configuration
	 *  for face detection. The config is returned as a mapping of strings to strings. Each key
	 *  is a variable name of the config and value is data it holds. Both need to be returned in
	 *  exact the same way in order for the loadConfig opertion to work. In the mean time
	 *  the config can be stored in the database. Alternatively use saveConfig() method for
	 *  libface to store config localy.
	 *
	 *  @return Returns std::map of string to string. this can be stored and loaded back.
	 */
	std::map<std::string, std::string> getConfig();

	/**
	 * Get the detection accuracy (see above for interpretation)
	 */
	double getDetectionAccuracy() const;

	/**
	 * Returns the image size (one dimension)
	 * recommended for face detection.
	 * Give the size of the available image, if possible.
	 * If the image is considerably larger, it will be rescaled automatically.
	 */
	int getRecommendedImageSizeForDetection(const CvSize& size = cvSize(0,0)) const;

	/**
	 * Returns the image size recommended for face recognition.
	 * Note that the returned size may not preserve pixel aspect ratio.
	 * Give the size of the available image, if possible.
	 * If the image is larger, it will be rescaled automatically.
	 */
	CvSize getRecommendedImageSizeForRecognition(const CvSize& size = cvSize(0,0)) const;

	/**
	 * Method for lading config file from the specified directory that contains libface.xml.
	 *
	 * @param dir Directory where the libface.xml is.
	 *
	 * @return Returns 0 is file loaded successfully.
	 */
	int loadConfig(const std::string& configDir);

	/**
	 *  Method for loading the config into libface. The config needs to be of the same format
	 *  as returned by the getConfig().
	 *
	 *  @param config - a mapping of string to string with keys being variables and values are
	 *  corresponding values.
	 *
	 *  @return Returns 0 if operation completed as expected.
	 */
	int loadConfig(const std::map<std::string, std::string>& config);

	std::vector<std::pair<int, float> > recognise(const IplImage* img, std::vector<Face*>* faces_p, int scaleFactor=1);


	/**
	 *  Method to recognise faces in the given image. The array of faces will be updated with
	 *  new id for each face.
	 *
	 * @param filename Filename of the image with the faces.
	 * @param faces Pointer to the std::vector of Face objects.
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster.
	 * Default not scaled (1). NOT USED at the moment.
	 * @return Vector of integer ID's that best match the input vector of Faces, in same order
	 */
	std::vector<std::pair<int, float> > recognise(const std::string& filename, std::vector<Face*>* faces_p, int scaleFactor=1);

	/**
	 *  Method to recognise faces in the given image. The array of faces will be updated with
	 *  new id for each face.
	 *
	 * @param filename Filename of the image with the faces.
	 * @param faces Pointer to the std::vector of Face objects.
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster.
	 * Default not scaled (1). NOT USED at the moment.
	 * @return Vector of integer ID's that best match the input vector of Faces, in same order
	 */
	std::vector<std::pair<int, float> > recognise(const char* image, std::vector<Face*>* faces_p, int width, int height, int step, int depth = IPL_DEPTH_8U, int channels = 1, int scaleFactor=1);

	/**
	 *  Method to recognise faces in the given image. The array of faces will be updated with
	 *  new id for each face.
	 *
	 * @param faces Pointer to the std::vector of Face objects.
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster.
	 * Default not scaled (1). NOT USED at the moment.
	 * @return Vector of integer ID's that best match the input vector of Faces, in same order
	 */
	std::vector<std::pair<int, float> > recognise( std::vector<Face*>* faces, int scaleFactor =1);


	/**
	 * Method for saving current configuration of the face recognition library.
	 * Creates/overwrites the libface.xml file.
	 *
	 * @param dir Directory to store the config file in.
	 *
	 * @return Returns 0 if the save was successful.
	 */
	int saveConfig(const std::string& configDir);

	/**
	 * Set the detection accuracy between 0 and 1.
	 * Trades speed vs accuracy: 0 is fast, 1 is slow but more accurate.
	 * Default is 0.8.
	 */
	void setDetectionAccuracy(double value);


	/** Method to update the library with faces from the picture specified.
	 *
	 * @param filename Filename of the image to load.
	 * @param faces Pointer to a sts::vector of Face objects. Default to NULL, meaning the whole image is face.
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster.
	 * Default not scaled (1). NOT USED at the moment.
	 *
	 * @return 0 is return if update was successful.
	 */
	int update(const std::string& filename, std::vector<Face*>* faces=NULL, int scaleFactor=1);

	/** Method to update the library with faces from the picture specified.
	 *
	 * @param image Image encoded as chars.
	 * @param faces Pointer to a std::vector of Face objects. Default to NULL, meaning the whole image is face.
	 * @param width Width of the image.
	 * @param height Height of the image.
	 * @param step Step.
	 * @param depth Depth of the image (default: IPL_DEPTH_8U).
	 * @param channels Number of channels (default: 1).
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster.
	 * Default not scaled (1). NOT USED at the moment.
	 *
	 * @return 0 is return if update was successful.
	 */
	int update(const char* image, std::vector<Face*>* faces, int width , int height , int step , int depth = IPL_DEPTH_8U, int channels = 1, int scaleFactor=1);

	/** Method to update the library with faces from the picture specified.
	 *
	 * @param filename Filename of the image to load.
	 * @param faces Pointer to a std::vector of Face objects. Default to NULL, meaning the whole image is face.
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster.
	 * Default not scaled (1). NOT USED at the moment.
	 *
	 * @return 0 is return if update was successful.
	 */
	int update(std::vector<Face*>* faces=NULL, int scaleFactor=1);

	/** Method to update the library with faces from the picture specified.
	 *
	 * @param img Pointer to the image where faces are.
	 * @param faces  Pointer to a std::vector of Face objects. Default to NULL, meaning the whole image is face.
	 * @param scaleFactor Allows to specify if image should be scaled. Make things faster. Default not scaled (1). NOT USED at the moment.
	 *
	 * @return 0 is return if update was successful.
	 */
	int update(const IplImage* img, std::vector<Face*>* faces, int scaleFactor = 1);

private:

	class LibFacePriv;
	LibFacePriv* const d;
};

} // namespace libface

#endif /* _LIBFACE_H_ */
