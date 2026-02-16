#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_occupant_generated.h"
}

#include "osi_occupant.pb.h"

#include <vector>

#include "osi_common_converter.hpp"

// ==========================
// Occupant - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::Occupant_::Classification> ConvertOccupantClassification(const osi3::Occupant::Classification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::Occupant_::ClassificationBuilder b(builder);
    
    if (proto.has_is_driver())
    	b.add_is_driver(proto.is_driver());
    
    if (proto.has_seat())
    	b.add_seat(static_cast<fbs::osi3::Occupant_::Classification_::Seat>(proto.seat()));
    
    if (proto.has_steering_control())
    	b.add_steering_control(static_cast<fbs::osi3::Occupant_::Classification_::SteeringControl>(proto.steering_control()));
    
    return b.Finish();
}

// ==========================
// Occupant
// ==========================

::flatbuffers::Offset<fbs::osi3::Occupant> ConvertOccupant(const osi3::Occupant& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::OccupantBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_classification())
    	b.add_classification(ConvertOccupantClassification(proto.classification(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    return b.Finish();
}
