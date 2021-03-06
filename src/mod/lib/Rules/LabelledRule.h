#ifndef MOD_LIB_RULES_LABELLED_RULE_H
#define MOD_LIB_RULES_LABELLED_RULE_H

#include <mod/lib/GraphMorphism/MatchConstraint.h>
#include <mod/lib/Rules/ConnectedComponent.h>
#include <mod/lib/Rules/GraphDecl.h>
#include <mod/lib/Rules/Properties/String.h>

#include <jla_boost/graph/dpo/FilteredGraphProjection.hpp>

#include <vector>

namespace mod {
namespace lib {
namespace Rules {
struct LabelledLeftGraph;
struct LabelledRightGraph;

class LabelledRule {
public: // LabelledGraphConcept, PushoutRuleConcept
	using GraphType = lib::Rules::GraphType;
public: // PushoutRuleConcept
	using LeftGraphType = jla_boost::GraphDPO::FilteredGraphProjection<const LabelledRule>;
	using ContextGraphType = LeftGraphType;
	using RightGraphType = LeftGraphType;
public: // LabelledGraphConcept
	using PropStringType = PropStringCore;
public: // Other
	using Vertex = boost::graph_traits<GraphType>::vertex_descriptor;
	using Edge = boost::graph_traits<GraphType>::edge_descriptor;
	using LeftMatchConstraint = GraphMorphism::MatchConstraint::Constraint<LeftGraphType>;
	using RightMatchConstraint = GraphMorphism::MatchConstraint::Constraint<RightGraphType>;
	using LabelledLeftType = LabelledLeftGraph;
	using LabelledRightType = LabelledRightGraph;
public:
	LabelledRule();
	LabelledRule(const LabelledRule &other, bool withConstraints);
	void initComponents(); // TODO: this is a huge hax
	void invert();
public: // LabelledGraphConcept, PushoutRuleConcept
	friend GraphType &get_graph(LabelledRule &r);
	friend const GraphType &get_graph(const LabelledRule &r);
public: // LabelledGraphConcept
	friend const PropStringType &get_string(const LabelledRule &r);
public: // PushoutRuleConcept
	friend const LeftGraphType &get_left(const LabelledRule &r);
	friend const ContextGraphType &get_context(const LabelledRule &r);
	friend const RightGraphType &get_right(const LabelledRule &r);
	friend jla_boost::GraphDPO::Membership membership(const LabelledRule &r, const Vertex &v);
	friend jla_boost::GraphDPO::Membership membership(const LabelledRule &r, const Edge &e);
	friend void put_membership(LabelledRule &r, const Vertex &v, jla_boost::GraphDPO::Membership m);
	friend void put_membership(LabelledRule &r, const Edge &e, jla_boost::GraphDPO::Membership m);
public:
	friend LabelledLeftType get_labelled_left(const LabelledRule &r);
	friend LabelledRightType get_labelled_right(const LabelledRule &r);
private:

	struct Projections {
		Projections(const LabelledRule &r);
	public:
		LeftGraphType left;
		ContextGraphType context;
		RightGraphType right;
	};
	std::unique_ptr<GraphType> g;
	mutable std::unique_ptr<Projections> projs;
public:
	mutable std::unique_ptr<PropStringType> pString;
	std::vector<std::unique_ptr<LeftMatchConstraint> > leftComponentMatchConstraints;
	std::vector<std::unique_ptr<RightMatchConstraint> >rightComponentMatchConstraints;
public:
	std::size_t numLeftComponents = -1, numRightComponents = -1;
	std::vector<std::size_t> leftComponents, rightComponents;
};

namespace detail {

struct LabelledSideGraph {
	using LabelledRule = lib::Rules::LabelledRule;
	using GraphType = jla_boost::GraphDPO::FilteredGraphProjection<const LabelledRule>;
	using ComponentFilter = ConnectedComponentFilter<GraphType, std::vector<std::size_t> >;
	using ComponentGraph = boost::filtered_graph<GraphType, ComponentFilter, ComponentFilter>;
public:
	LabelledSideGraph(const LabelledRule &r, jla_boost::GraphDPO::Membership m);
public:
	const LabelledRule &r;
	const jla_boost::GraphDPO::Membership m;
protected:
	mutable std::vector<std::vector<boost::graph_traits<GraphType>::vertex_descriptor> > vertex_orders;
};

} // namespace detail

struct LabelledLeftGraph : detail::LabelledSideGraph {
	using Base = detail::LabelledSideGraph;
	using PropStringType = LabelledRule::PropStringType::LeftType;
public:
	explicit LabelledLeftGraph(const LabelledRule &r);
	friend const Base::GraphType &get_graph(const LabelledLeftGraph &g);
	friend PropStringType get_string(const LabelledLeftGraph &g);
public:
	friend const std::vector<std::unique_ptr<LabelledRule::LeftMatchConstraint> > &
	get_match_constraints(const LabelledLeftGraph &g);
public:
	friend std::size_t get_num_connected_components(const LabelledLeftGraph &g);
	friend Base::ComponentGraph get_component_graph(std::size_t i, const LabelledLeftGraph &g);
public:
	friend const std::vector<boost::graph_traits<GraphType>::vertex_descriptor>&
	get_vertex_order_component(std::size_t i, const LabelledLeftGraph &g);
};

struct LabelledRightGraph : detail::LabelledSideGraph {
	using Base = detail::LabelledSideGraph;
	using PropStringType = LabelledRule::PropStringType::RightType;
public:
	explicit LabelledRightGraph(const LabelledRule &r);
	friend const Base::GraphType &get_graph(const LabelledRightGraph &g);
	friend PropStringType get_string(const LabelledRightGraph &g);
public:
	friend const std::vector<std::unique_ptr<LabelledRule::RightMatchConstraint> > &
	get_match_constraints(const LabelledRightGraph &g);
public:
	friend std::size_t get_num_connected_components(const LabelledRightGraph &g);
	friend Base::ComponentGraph get_component_graph(std::size_t i, const LabelledRightGraph &g);
public:
	friend const std::vector<boost::graph_traits<GraphType>::vertex_descriptor>&
	get_vertex_order_component(std::size_t i, const LabelledRightGraph &g);
};

} // namespace Rules
} // namespace lib
} // namespace mod

#endif /* MOD_LIB_RULES_LABELLED_RULE_H */
