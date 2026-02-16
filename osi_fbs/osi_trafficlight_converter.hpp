#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_trafficlight_generated.h"
}

#include "osi_trafficlight.pb.h"

#include <vector>

#include "osi_common_converter.hpp"

// ==========================
// TrafficLight - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficLight_::Classification> ConvertTrafficLightClassification(const osi3::TrafficLight::Classification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficLight_::ClassificationBuilder b(builder);
    
    if (proto.has_color())
    	b.add_color(static_cast<fbs::osi3::TrafficLight_::Classification_::Color>(proto.color()));
    
    if (proto.has_icon())
    	b.add_icon(static_cast<fbs::osi3::TrafficLight_::Classification_::Icon>(proto.icon()));
    
    if (proto.has_mode())
    	b.add_mode(static_cast<fbs::osi3::TrafficLight_::Classification_::Mode>(proto.mode()));
    
    if (proto.has_counter())
    	b.add_counter(proto.counter());
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& item : proto.assigned_lane_id())
        assigned_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_assigned_lane_id(builder.CreateVector(assigned_lane_id));
    
    if (proto.has_is_out_of_service())
    	b.add_is_out_of_service(proto.is_out_of_service());
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& item : proto.logical_lane_assignment())
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(item, builder));
    b.add_logical_lane_assignment(builder.CreateVector(logical_lane_assignment));
    
    return b.Finish();
}

// ==========================
// TrafficLight
// ==========================

::flatbuffers::Offset<fbs::osi3::TrafficLight> ConvertTrafficLight(const osi3::TrafficLight& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::TrafficLightBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_base())
    	b.add_base(ConvertBaseStationary(proto.base(), builder));
    
    if (proto.has_classification())
    	b.add_classification(ConvertTrafficLightClassification(proto.classification(), builder));
    
    if (proto.has_model_reference())
    	b.add_model_reference(builder.CreateString(proto.model_reference()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    if (proto.has_color_description())
    	b.add_color_description(ConvertColorDescription(proto.color_description(), builder));
    
    return b.Finish();
}
