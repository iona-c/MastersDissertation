#ifndef PROJECTION
#define PROJECTION

#include "Image.h"
#include "Volume.h"

/**
 * The Projection class provides functionality to apply various projection techniques to a Volume object.
 * It supports Maximum Intensity Projection (MIP), Minimum Intensity Projection (MinIP), and Average
 * Intensity Projection (AIP). These methods are used to project a 3D volume onto a 2D plane, typically
 * for visualization or analysis purposes.
 *
 * Enums:
 *   Proj: Defines the types of projections available.
 *     - projMIP: Represents Maximum Intensity Projection.
 *     - projMinIP: Represents Minimum Intensity Projection.
 *     - projAIP: Represents Average Intensity Projection.
 *
 * Public Method:
 *   void apply(Proj method, Volume& volume):
 *     Applies the specified projection method to the given volume.
 *     @param method The projection method to apply, specified as a value from the Proj enum.
 *     @param volume A reference to the Volume object on which the projection will be applied.
 *
 * Private Methods:
 *   void MIP(Volume& volume):
 *     Applies Maximum Intensity Projection to the given volume.
 *     @param volume A reference to the Volume object on which MIP will be applied.
 *
 *   void AIP(Volume& volume):
 *     Applies Average Intensity Projection to the given volume.
 *     @param volume A reference to the Volume object on which AIP will be applied.
 *
 *   void MinIP(Volume& volume):
 *     Applies Minimum Intensity Projection to the given volume.
 *     @param volume A reference to the Volume object on which MinIP will be applied.
 * 
 * @author Prayush Udas
 */
class Projection{

    public:
        enum Proj{
            projMIP,
            projMinIP,
            projAIP
        };
        void apply(Proj method, Volume& volume);

    private:
        void MIP(Volume& volume);
        void AIP(Volume& volume);
        void MinIP(Volume& volume);
};

#endif
