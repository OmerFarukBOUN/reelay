#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_referenceline_generated.h"
}

#include "osi_referenceline.pb.h"

#include <vector>

#include "osi_common_converter.hpp"

// ==========================
// ReferenceLine - ReferenceLinePoint
// ==========================

::flatbuffers::Offset<fbs::osi3::ReferenceLine_::ReferenceLinePoint> ConvertReferenceLineReferenceLinePoint(const osi3::ReferenceLine::ReferenceLinePoint& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ReferenceLine_::ReferenceLinePointBuilder b(builder);
    
    if (proto.has_world_position())
    	b.add_world_position(ConvertVector3d(proto.world_position(), builder));
    
    if (proto.has_s_position())
    	b.add_s_position(proto.s_position());
    
    if (proto.has_t_axis_yaw())
    	b.add_t_axis_yaw(proto.t_axis_yaw());
    
    return b.Finish();
}

// ==========================
// ReferenceLine
// ==========================

::flatbuffers::Offset<fbs::osi3::ReferenceLine> ConvertReferenceLine(const osi3::ReferenceLine& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::ReferenceLineBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::ReferenceLine_::Type>(proto.type()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ReferenceLine_::ReferenceLinePoint>> poly_line;
    for (const auto& item : proto.poly_line())
        poly_line.push_back(ConvertReferenceLineReferenceLinePoint(item, builder));
    b.add_poly_line(builder.CreateVector(poly_line));
    
    return b.Finish();
}
