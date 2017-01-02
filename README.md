[s1]: http://i.imgur.com/m9Rl9XP.png "Original Sphere"
[s2a]: http://i.imgur.com/ZN1WQ54.png "Spheres Under Light"
[s2b]: http://i.imgur.com/JfQf2j0.png "Objects Under Light"
[s3]: http://i.imgur.com/JfQf2j0.png "Needle Map"
[s4]: http://i.imgur.com/wfeansR.png "Albedo Map"

# Shapes From Shading

**Shape From Shading** is an application of **Photometric Stereo** which is
a technique in computer vision for estimating the surface normals of objects
by observing that object under different lighting conditions. It is based on
the fact that the amount of light reflected by a surface is dependent on the
orientation of the surface in relation to the light source and the observer.
By measuring the amount of light reflected into a camera, the space of
possible surface orientations is limited. Given enough light sources from
different angles, the surface orientation may be constrained to a single
orientation or even over-constrained.

A key assumption used here is that the surfaces of the objects are purely *Lambertian*.

## S1

This program will locate the sphere in question, while also computing its
center and radius. The sphere image used is `sphere0.pgm`.

![s1]

## S2

This program will compute the direction of the light source hitting the
sphere. This will be computed for sphere `sphere1`, `sphere2`, and `sphere3`.
The goal is to use these normals for the actual `object.pgm` images, which are
placed in the same location and under the same lighting conditions.

![s2a]

![s2b]

## S3

Now that we have the normals for the directions of the light source, we can
compute the normals for all points on the object. However, we will only
compute the normal for the points which are visible on all three of the
`object.pgm` images. The result of this program can actually output a visual
representation of the object in 3D using something called a `needle map` as
shown below.

![s3]

## S4

In addition to the `needle map`, we can also compute something called the
`albedo map`. The albedo actually gives us information about the surface
of the object at that point. Here's the albedo map of the object:

![s4]

# Program Execution

`./s1 <input original image> <input threshold> <output parameters file>`

  - threshold: 90

`./s2 <input parameters file> <image 1> <image 2> <image 3> <output directions file>`

#### Formulas in S2

    Sphere:
      x^2 + y^2 + z^2 = r^2
      z = sqrt(r^2 - (x^2 + y^2))
    p = dz/dx = -x / sqrt(r^2 - (x^2 + y^2))
    q = dz/dy = -y / sqrt(r^2 - (x^2 + y^2))
    n = (p,q,1) / sqrt(p^2 + q^2 + 1)

`./s3 <input directions> <image 1> <image 2> <image 3> <step> <threshold> <output>`

  - step: 10
  - threshold: 100

`./s4 <input directions> <image 1> <image 2> <image 3> <threshold> <output>`
  - threshold: 85
