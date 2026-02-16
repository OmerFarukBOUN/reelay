#include <flatbuffers/flatbuffers.h>
namespace fbs
{
#include "osi_roadmarking_generated.h"
}

#include "osi_roadmarking.pb.h"

#include <vector>

#include "osi_common_converter.hpp"
#include "osi_trafficsign_converter.hpp"

// ==========================
// RoadMarking - Classification
// ==========================

::flatbuffers::Offset<fbs::osi3::RoadMarking_::Classification> ConvertRoadMarkingClassification(const osi3::RoadMarking::Classification& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::RoadMarking_::ClassificationBuilder b(builder);
    
    if (proto.has_type())
    	b.add_type(static_cast<fbs::osi3::RoadMarking_::Classification_::Type>(proto.type()));
    
    if (proto.has_traffic_main_sign_type())
    	b.add_traffic_main_sign_type(static_cast<fbs::osi3::TrafficSign_::MainSign_::Classification_::Type>(proto.traffic_main_sign_type()));
    
    if (proto.has_monochrome_color())
    	b.add_monochrome_color(static_cast<fbs::osi3::RoadMarking_::Classification_::Color>(proto.monochrome_color()));
    
    if (proto.has_value())
    	b.add_value(ConvertTrafficSignValue(proto.value(), builder));
    
    if (proto.has_value_text())
    	b.add_value_text(builder.CreateString(proto.value_text()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::Identifier>> assigned_lane_id;
    for (const auto& item : proto.assigned_lane_id())
        assigned_lane_id.push_back(ConvertIdentifier(item, builder));
    b.add_assigned_lane_id(builder.CreateVector(assigned_lane_id));
    
    if (proto.has_is_out_of_service())
    	b.add_is_out_of_service(proto.is_out_of_service());
    
    if (proto.has_country())
    	b.add_country(builder.CreateString(proto.country()));
    
    if (proto.has_country_revision())
    	b.add_country_revision(builder.CreateString(proto.country_revision()));
    
    if (proto.has_code())
    	b.add_code(builder.CreateString(proto.code()));
    
    if (proto.has_sub_code())
    	b.add_sub_code(builder.CreateString(proto.sub_code()));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::LogicalLaneAssignment>> logical_lane_assignment;
    for (const auto& item : proto.logical_lane_assignment())
        logical_lane_assignment.push_back(ConvertLogicalLaneAssignment(item, builder));
    b.add_logical_lane_assignment(builder.CreateVector(logical_lane_assignment));
    
    return b.Finish();
}

// ==========================
// RoadMarking
// ==========================

::flatbuffers::Offset<fbs::osi3::RoadMarking> ConvertRoadMarking(const osi3::RoadMarking& proto,
                        flatbuffers::FlatBufferBuilder& builder)
{
    fbs::osi3::RoadMarkingBuilder b(builder);
    
    if (proto.has_id())
    	b.add_id(ConvertIdentifier(proto.id(), builder));
    
    if (proto.has_base())
    	b.add_base(ConvertBaseStationary(proto.base(), builder));
    
    if (proto.has_classification())
    	b.add_classification(ConvertRoadMarkingClassification(proto.classification(), builder));
    
    std::vector<::flatbuffers::Offset<fbs::osi3::ExternalReference>> source_reference;
    for (const auto& item : proto.source_reference())
        source_reference.push_back(ConvertExternalReference(item, builder));
    b.add_source_reference(builder.CreateVector(source_reference));
    
    if (proto.has_color_description())
    	b.add_color_description(ConvertColorDescription(proto.color_description(), builder));
    
    return b.Finish();
}
