from math import *
from openalea.plantgl.all import Vector3, Vector4, Matrix4, cross
import numpy as np

class PyZBufferRenderer:
    def __init__(self, width, height):
        self.projectionMatrix = Matrix4.IDENTITY
        self.width = width
        self.height = height
        self.depths = np.ones((width,height))
        self.image = np.zeros((width,height,3))


    def setPerspective(self, angleOfView, aspectRatio, near, far):
        top = near *tan(radians(angleOfView)/2.)
        bottom = -top
        right = top * aspectRatio
        left = -right
        return self.setFrustrum(left, right, bottom, top, near, far)

    def setFrustrum(self, left, right, bottom, top, near, far):
        rl = float(right-left)
        tb = float(top - bottom)
        fn = float(far-near)
        self.projectionMatrix = Matrix4((2*near/rl, 0, 0, 0, # -near*(right+left)/rl,
                                        0, 2*near/tb, 0,  0, #-near*(top+bottom)/tb,
                                        0, 0, - (far+near)/fn, 2*far*near/(-fn),
                                        0, 0, - 1, 0))
        return self.projectionMatrix


    def setOrthographic(self, left, right, bottom, top, near, far):
        rl = float(right-left)
        tb = float(top - bottom)
        fn = float(far-near)
        self.projectionMatrix = Matrix4((2/rl, 0, 0,  -(right+left)/rl,
                                         0, 2/tb, 0,  -(top+bottom)/tb,
                                         0, 0, - 2/fn, -(far+near)/fn,
                                         0, 0, 0, 1))
        return self.projectionMatrix

    def lookAt(self, eyePosition3D, center3D, upVector3D):
        forward = Vector3(center3D) - Vector3(eyePosition3D)
        forward.normalize()
        upVector3D = Vector3(upVector3D)
        upVector3D.normalize()
        side = cross(forward, upVector3D)
        side.normalize()
        up = cross(side, forward)
        up.normalize()
        m = Matrix4(side, up, forward, -eyePosition3D)
        self.worldToCamera = m.inverse()
        return m

    def projectPointToCameraPlane(self, position):
        p = Vector4(Vector3(position) ,1)
        p = self.worldToCamera * p
        p = self.projectionMatrix * p
        if abs(p.w) > 1e-3 : return p.project()
        return p

    def _projectPointToImage(self, position):
        x = (position[0]+1)*0.5 * self.width
        y = (1-(position[1]+1)*0.5) * self.height
        x = int(x)
        y = int(y)
        return x,y,position[2]

    def _isInUnitSquare(self, *positions):
        if len(positions) == 1:
            position = positions[0]
            return -1 <= position[0] <= 1 and -1 <= position[1] <= 1 and -1 <= position[2] <= 1
        else:
            xmin = min([p[0] for p in positions])
            xmax = max([p[0] for p in positions])
            ymin = min([p[1] for p in positions])
            ymax = max([p[1] for p in positions])
            zmin = min([p[2] for p in positions])
            zmax = max([p[2] for p in positions])
            return not (xmin > 1 or xmax < -1 or ymin > 1 or ymax < -1 or zmin > 1 or zmax < -1)


    def projectPointToImage(self, position):
        npos = self.projectPointToCameraPlane(position)
        if self._isInUnitSquare(npos):
            return self._projectPointToImage(npos)

    def renderPoint(self, position, color = (255,255,255), pointsize = 1):
        raster = self.projectPointToImage(position)
        print('Raster', raster)
        if raster:
            self._renderRaster(raster[0],raster[1],raster[2], color, pointsize)


    def _renderRaster(self, x,y,z, color = (255,255,255), pointsize = 1):
            print(x,y,z,color,pointsize)
            for xv in range(max(0,x-pointsize+1), min(self.width,x+pointsize)):
                for yv in range(max(0,y-pointsize+1), min(self.height,y+pointsize)):
                    if z < self.depths[yv,xv]:
                        self.image[yv,xv,:] = color
                        self.depths[yv,xv] = z

    def renderLine(self, p0, p1, color = (255,255,255), pointsize = 1):
        def lineLow(p0, p1):
            dx = p1[0] - p0[0]
            dy = p1[1] - p0[1]
            dz = p1[2] - p0[2]
            Dz = dz/dx

            yi = 1
            if dy < 0:
                yi = -1
                dy = -dy

            D = 2 * dy - dx
            y = p0[1]
            z = p0[2]

            for x in range(p0[0],p1[0]+1):
                if 0 <= x < self.width:
                    self._renderRaster(x,y,z, color, pointsize)
                if D > 0:
                    y = y + yi
                    D -=  2*dx
                D += 2*dy
                z += Dz

        def lineHigh(p0,p1):
            dx = p1[0] - p0[0]
            dy = p1[1] - p0[1]
            dz = p1[2] - p0[2]
            Dz = dz/dy

            xi = 1
            if dx < 0:
                xi = -1
                dx = -dx

            D = 2 * dx - dy
            x = p0[0]
            z = p0[2]

            for y in range(p0[1],p1[1]+1):
                if 0 <= y < self.height:
                    self._renderRaster(x,y,z, color, pointsize)
                if D > 0:
                    x = x + xi
                    D -=  2*dy
                D += 2*dx
                z += Dz
        def line(p0,p1):
            if abs(p1[1] - p0[1]) < abs(p1[0] - p0[0]):
                if p0[0] > p1[0]:
                    plotLineLow(p1,p0)
                else:
                    plotLineLow(p0,p1)
            else:
                if p0[1] > p1[1]:
                    lineHigh(p1,p0)
                else:
                    lineHigh(p0,p1)

        npos0 = self.projectPointToCameraPlane(p0)
        npos1 = self.projectPointToCameraPlane(p1)
        if self._isInUnitSquare(npos0, npos1) :
            npos0 = self._projectPointToImage(npos0)
            npos1 = self._projectPointToImage(npos1)
            line(npos0, npos1)




    def view(self):
        import matplotlib.pyplot as plt
        plt.imshow(self.image)
        plt.show()

def horizontal_corners(bbx):
    from itertools import product
    from openalea.plantgl.math import Vector3
    
    lc = bbx.lowerLeftCorner
    uc = bbx.upperRightCorner
    mh = (lc.z+uc.z)/2
    return [Vector3(uc.x,uc.y,mh),Vector3(lc.x,uc.y,mh), Vector3(lc.x,lc.y,mh),Vector3(uc.x,lc.y,mh)]

def findClosestPoint(bbx, pos, dir):
    hcorners = horizontal_corners(bbx)
    closestpoint = Polyline(hcorners+[hcorners[0]]).findClosest(pos)[0]
    mindist = dot(closestpoint-pos, dir)
    for pt in hcorners:
        ldist = dot(pt-pos, dir)
        if ldist < mindist:
            closestpoint = pt 
            mindist = ldist
    return closestpoint, mindist

def scan_infos(scene,  scan_positions, debug = True):
    bbx = BoundingBox(scene)
    center = bbx.getCenter()
    corners = bbx.corners()
    lateral = Vector3(bbx.getSize().x,bbx.getSize().y,0)
    laterals = [Vector3(lateral.x,lateral.y,0),Vector3(-lateral.x,lateral.y,0),
                Vector3(-lateral.x,-lateral.y,0),Vector3(lateral.x,-lateral.y,0)]
    up = Vector3(0,0,1)
    shift = 2*20
    if debug:
        print('scan_positions:',scan_positions)
    centers = []
    scan_horizontal_angles = []
    scan_vertical_angles = []
    scan_lefts = []
    znears = []
    zfars = []
    for pos in scan_positions:
        pos = Vector3(pos)
        h_angles = list(sorted([ (i,angle(center-pos,center+lat-pos, up)) for i,lat in enumerate(laterals) ], key=itemgetter(1)))
        min_angle, max_angle = h_angles[0][1], h_angles[-1][1]
        center_angle = (max_angle + min_angle)/2 
        ncenter = pos + Matrix3.axisRotation(up, center_angle) * (center - pos)
        centers.append(ncenter)
        if debug : print('center:',degrees(angle(ncenter-pos,center-pos, up)), ncenter)
        viewdirection = direction(ncenter-pos)

        h_angles = list(sorted([ (i,angle(ncenter-pos,center+lat-pos, up)) for i,lat in enumerate(laterals) ], key=itemgetter(1)))
        min_angle, max_angle = h_angles[0][1], h_angles[-1][1]
        extend_angle = (max_angle - min_angle)
        scan_horizontal_angles.append(extend_angle)

        if debug : 
            print(degrees(min_angle), degrees(max_angle))
            print('hangle:',degrees(extend_angle))

        closestpoint, mindist = findClosestPoint(bbx, pos, viewdirection)
        print(mindist, norm(closestpoint-pos))

        print(closestpoint, pos, bbx.getSize())
        vangle = 2*angle(viewdirection*mindist,viewdirection*mindist+(up*bbx.getSize().z)) 
        scan_vertical_angles.append(vangle)

        if debug : print('vangle',degrees(vangle))

        znears.append(mindist*0.95)
        zfars.append(max(dot(c - pos,viewdirection) for c in corners)*1.05)

        if debug : print("N&F:",znears[-1],zfars[-1])

    return scan_positions, scan_horizontal_angles, scan_vertical_angles, znears, zfars, centers, bbx

def from_bbox(bbox, azimut = 0, elevation = 0, zoom = 1, imagesize = (400,400), vertical_view_angle = 30 ):
    #direction = Vector3.
    center = bbx.getCenter()
    corners = bbx.corners()
    lateral = Vector3(bbx.getSize().x,bbx.getSize().y,0)
    laterals = [Vector3(lateral.x,lateral.y,0),Vector3(-lateral.x,lateral.y,0),
                Vector3(-lateral.x,-lateral.y,0),Vector3(lateral.x,-lateral.y,0)]



def test():
    renderer = ZBufferRenderer(10, 10)
    print(renderer.setPerspective(30, 1, 0.1, 100))
    print(renderer.lookAt(Vector3(1,0,1), (0,0,0),(0,0,1)))
    print(renderer.projectionMatrix)
    print(renderer.renderPoint((0,0,0)))
    renderer.view()


if __name__ == '__main__':
    renderer = ZBufferRenderer(100, 100)
    print(renderer.setPerspective(30, 1, 0.1, 100))
    print(renderer.lookAt(Vector3(5,0,1), (0,0,0),(0,0,1)))
    print(renderer.projectionMatrix)
    renderer.renderPoint((0,0,0), (255,0,0), 3)
    renderer.renderLine((0,0,-1), (0,0,1), (0,255,0) )
    renderer.view()
