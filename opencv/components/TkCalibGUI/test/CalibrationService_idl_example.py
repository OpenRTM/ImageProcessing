#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- Python -*-

"""
 @file CalibrationService_idl_example.py
 @brief Python example implementations generated from CalibrationService.idl
 @date $Date$

 @author Noriaki Ando <n-ando@aist.go.jp>

"""

import omniORB
from omniORB import CORBA, PortableServer
import ImageCalibService, ImageCalibService__POA



class CalibrationService_i (ImageCalibService__POA.CalibrationService):
    """
    @class CalibrationService_i
    Example class implementing IDL interface ImageCalibService.CalibrationService
    """

    def __init__(self):
        """
        @brief standard constructor
        Initialise member variables here
        """
        pass

    # void setImageNumber(in short num)
    def setImageNumber(self, num):
        raise CORBA.NO_IMPLEMENT(0, CORBA.COMPLETED_NO)
        # *** Implement me
        # Must return: None

    # short getImageNumber()
    def getImageNumber(self):
        raise CORBA.NO_IMPLEMENT(0, CORBA.COMPLETED_NO)
        # *** Implement me
        # Must return: result

    # RTC::CameraImage captureCalibImage(in short num)
    def captureCalibImage(self, num):
        raise CORBA.NO_IMPLEMENT(0, CORBA.COMPLETED_NO)
        # *** Implement me
        # Must return: result

    # RTC::CameraImage getCalibImage(in short num)
    def getCalibImage(self, num):
        raise CORBA.NO_IMPLEMENT(0, CORBA.COMPLETED_NO)
        # *** Implement me
        # Must return: result

    # ImageList getCalibImages()
    def getCalibImages(self):
        raise CORBA.NO_IMPLEMENT(0, CORBA.COMPLETED_NO)
        # *** Implement me
        # Must return: result

    # boolean removeCalibImage(in short num)
    def removeCalibImage(self, num):
        raise CORBA.NO_IMPLEMENT(0, CORBA.COMPLETED_NO)
        # *** Implement me
        # Must return: result

    # RTC::CameraInfo getCalibParameter()
    def getCalibParameter(self):
        raise CORBA.NO_IMPLEMENT(0, CORBA.COMPLETED_NO)
        # *** Implement me
        # Must return: result


if __name__ == "__main__":
    import sys
    
    # Initialise the ORB
    orb = CORBA.ORB_init(sys.argv)
    
    # As an example, we activate an object in the Root POA
    poa = orb.resolve_initial_references("RootPOA")

    # Create an instance of a servant class
    servant = CalibrationService_i()

    # Activate it in the Root POA
    poa.activate_object(servant)

    # Get the object reference to the object
    objref = servant._this()
    
    # Print a stringified IOR for it
    print(orb.object_to_string(objref))

    # Activate the Root POA's manager
    poa._get_the_POAManager().activate()

    # Run the ORB, blocking this thread
    orb.run()

