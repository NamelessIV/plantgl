from openalea.core.external import *
import openalea.core.interface as intface
from openalea.plantgl import math as mt
from openalea.plantgl import scenegraph as sg
 


class SceneNode(Node):
    sg.Scene.__doc__

    def __init__(self):

        Node.__init__(self)

        self.add_input( name = "objects", interface = None ) 
        self.add_output( name = "scene", interface = None) 

    def __call__(self, inputs):
        """ inputs is the list of input values """		
        scene= inputs[0]
        if scene:
            return (sg.Scene(scene),)
        else:
            return sg.Scene()


def isWritableProperty(obj,name):
    try:
        prop = getattr(obj,name)
        if not isinstance(prop,property):
            return False
        return prop.fset != None
    except:
        return False
    
def getproperties(obj):
    return [i for i in dir(obj) if isWritableProperty(obj,i)]

def hasdefaultprop(obj,prop):
    try:
        default = getattr(obj,'is'+prop[0].upper()+prop[1:]+'ToDefault')
        return default != None
    except:
        return False


class PglNode(Node):
    pgltype = None
    def __init__(self):
        Node.__init__(self)
        self.add_output( name='obj', interface=None)
        self.add_pglinputs()
    def add_pglinputs(self):
        prop = getproperties(self.pgltype)
        prop.remove('name')
        defaultval = []
        try:
            instance = self.pgltype()
            for p in prop:
                defaultval.append(getattr(instance,p))
        except:
            pass
        self.add_input(name = 'name', interface=IStr, value ='')
        interfacedict = intface.TypeInterfaceMap()
        interfacedict[sg.Color3] = IRGBColor        
        for i,p in enumerate(prop):
            args = { 'name' : p }
            if len(defaultval) > 0:
                interface = interfacedict.get(type(defaultval[i]),None)
                if interface:
                    args ['interface'] = interface
                args['value'] = defaultval[i]
            self.add_input(**args)        
    def __call__(self, inputs):
        args = {}
        for desc in self.input_desc:
            key = desc['name']
            x = self.get_input(key)
            if x is not None:  # selects the input arguments
                args[key] = x
        name = args.get('name',None)
        del args['name']
        obj = self.pgltype(**args)
        obj.name = name
        return (obj,)



from new import classobj
        
def generatePglNode(pgltype):
    globals()[pgltype.__name__+'Node'] = classobj(pgltype.__name__+'Node',(PglNode,),{'pgltype':pgltype})

def getSceneGraphNodes(baseclass):
    cl = []
    for i in dir(sg):
        c = getattr(sg,i)
        try:
          if issubclass(c,baseclass):
            try:
                i = c()
                cl.append(c)
            except RuntimeError,e:
                # Cannot instanciate this class. Boost return runtime error.
                pass
            except:
                cl.append(c)
        except:
          pass
    cl.sort(lambda x,y : cmp(x.__name__,y.__name__))
    return cl
    
PGLCLASS = getSceneGraphNodes((sg.Geometry,sg.Shape3D,sg.Appearance,sg.Scene))

#[sg.Sphere,sg.Cylinder,sg.Cone,sg.Frustum,sg.Paraboloid,sg.AsymmetricHull,sg.ExtrudedHull,sg.FaceSet,
#sg.AmapSymbol, sg.QuadSet, sg.TriangleSet, sg.PointSet, sg.Polyline, sg.Group, sg.BezierCurve, sg.NurbsCurve,
#sg.Box, sg.Extrusion, sg.BezierPatch, sg.NurbsPatch, sg.ElevationGrid, sg.BezierCurve2D, sg.NurbsCurve2D,
#sg.Polyline2D, sg.PointSet2D, sg.Disc, sg.Revolution, sg.Swung, sg.Text, sg.Font, sg.IFS, sg.AxisRotated,
#sg.EulerRotated, sg.Oriented, sg.Scaled, sg.Translated, sg.Tapered, sg.Shape, sg.Material, sg.ImageTexture]

map(lambda x : generatePglNode(x),PGLCLASS)
