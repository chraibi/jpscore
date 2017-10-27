#!/usr/bin/env python
import numpy as np
import matplotlib.cm as cm
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
import argparse
import sys,glob
import os
# example usage:
# python scripts/_Plot_profiles.py -p demos/corridor_JPScore/Output/Fundamental_Diagram/Classical_Voronoi/field -b 10 -n trajectory.xml -e 100 -x1 0 -x2 23 -y1 0 -y2 4

def getParserArgs():
        parser = argparse.ArgumentParser(description='Plot Profiles of density, velocity and flow')
        parser.add_argument("-p", "--pathfile", default="./", help='path to the field directory (some/path/field)')
        parser.add_argument("-n", "--nametraj", required=True, help='name of the trajectory file')
        parser.add_argument("-b", "--beginsteady", type=int, required=True, help='the frame for the beginning of steady state')
        parser.add_argument("-e", "--endsteady", type=int, required=True, help='the frame for the ending of steady state')
        parser.add_argument("-x1", "--geominx", type=float, required=True, help='the minmum x of the geometry')
        parser.add_argument("-x2", "--geomaxx", type=float, required=True, help='the maxmum x of the geometry')
        parser.add_argument("-y1", "--geominy", type=float, required=True, help='the minmum y of the geometry')
        parser.add_argument("-y2", "--geomaxy", type=float, required=True, help='the maxmum y of the geometry')
        args = parser.parse_args()
        return args

if __name__ == '__main__':
    args = getParserArgs()
    pathfile = args.pathfile
    sys.path.append(pathfile)
    nametraj = args.nametraj
    beginsteady = args.beginsteady
    endsteady = args.endsteady
    geominX = args.geominx
    geomaxX = args.geomaxx
    geominY = args.geominy
    geomaxY = args.geomaxy

    print("pathfile: %s" % pathfile)
    print("namtraj: %s" %nametraj)
    print("Steadystate: [%d, %d]" %(beginsteady, endsteady))
    print("geometry: X [%.1f, %.1f], Y=[%.1f, %.1f]" %
          (geominX, geomaxX, geominY, geomaxY))

    density_files = os.path.join(
        pathfile, "density", "*%s*%d.dat" %
        (nametraj,beginsteady))
    print("looking for %s" % density_files)
    f_Voronoi = glob.glob(density_files)
    # get the shape of the matrices
    if len(f_Voronoi) == 0:
        sys.exit("no Voronoi files found in %s" % pathfile)

    shape = np.shape(np.loadtxt(f_Voronoi[0]))
    density = np.zeros(shape)
    velocity = np.zeros(shape)
    #-------------------------density profile----------------
    fig = plt.figure(
        figsize=(16*(geomaxX-geominX)/(geomaxY-geominY)+2, 16), dpi=100)
    ax1 = fig.add_subplot(111, aspect='1')
    plt.rc("font", size=40)
    for j in range(beginsteady,endsteady):
        density_file = os.path.join(
            pathfile, "density", "Prf_d_%s_id_1_%.5d.dat" % (nametraj, j) )
        print("loading: %s" % density_file)
        density += np.loadtxt(density_file)

    density=density/(endsteady-beginsteady)

    im = plt.imshow(density, cmap=cm.jet,
                    interpolation='nearest',origin='lower',
                    vmin=0,vmax=np.amax(density),
                    extent=[geominX,geomaxX,geominY,geomaxY])
    ax1.set_xlabel("x [m]")
    ax1.set_ylabel("y [m]")
    divider = make_axes_locatable(ax1)
    cax = divider.append_axes("right", size="2.5%", pad=0.3)
    cb=plt.colorbar(im,cax=cax)
    cb.set_label('Density [$m^{-2}$]')
    density_figname = os.path.join(pathfile, "profile_rho_%s.png"% nametraj)
    plt.savefig(density_figname)
    plt.close()

    #-----------------velocity profile-----------------------------------
    fig = plt.figure(
        figsize=(16*(geomaxX-geominX)/(geomaxY-geominY)+2, 16),
        dpi=100)
    ax1 = fig.add_subplot(111, aspect='1')
    plt.rc("font", size=40)
    for j in range(beginsteady,endsteady):
        velocity_file = os.path.join(
            pathfile, "velocity", "Prf_v_%s_id_1_%.5d.dat" % (nametraj, j) )
        print("loading: %s" % velocity_file)
        velocity += np.loadtxt(velocity_file)

    velocity=velocity/(endsteady-beginsteady)
    im = plt.imshow(velocity,
                    cmap=cm.jet,
                    interpolation='nearest',
                    origin='lower',
                    vmin=0,vmax=np.amax(velocity),
                    extent=[geominX,geomaxX,geominY,geomaxY])
    ax1.set_xlabel("x [m]")
    ax1.set_ylabel("y [m]")
    divider = make_axes_locatable(ax1)
    cax = divider.append_axes("right", size="2.5%", pad=0.3)
    cb=plt.colorbar(im,cax=cax)
    cb.set_label('Velocity [$m/s$]')
    velocity_figname = os.path.join(pathfile, "profile_v_%s.png"% nametraj)
    plt.savefig(velocity_figname)
    plt.close()

#-------------------flow profile-------------------------
    fig = plt.figure(
        figsize=(16*(geomaxX-geominX)/(geomaxY-geominY)+2, 16), dpi=100)
    ax1 = fig.add_subplot(111, aspect='1')
    plt.rc("font", size=40)
    flow=density*velocity
    im = plt.imshow(flow,
                    cmap=cm.jet, interpolation='nearest',origin='lower',
                    vmin=0,vmax=np.amax(flow),
                    extent=[geominX,geomaxX,geominY,geomaxY])
    ax1.set_xlabel("x [m]")
    ax1.set_ylabel("y [m]")
    divider = make_axes_locatable(ax1)
    cax = divider.append_axes("right", size="2.5%", pad=0.3)
    cb=plt.colorbar(im,cax=cax)
    cb.set_label('Specific flow [$1/m \cdot s$]')
    figname = "%s/profile_flux_%s.png"%(pathfile,nametraj)
    flux_figname = os.path.join(pathfile, "profile_flux_%s.png"% nametraj)

    plt.savefig(flux_figname)
    plt.close()


    # print some infos
    print("\n\n============= \n---> %s" % density_figname)
    print("---> %s" % velocity_figname)
    print("---> %s\n============= " % flux_figname)

