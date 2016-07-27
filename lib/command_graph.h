#ifndef COMMAND_GRAPH_H
#define COMMAND_GRAPH_H

#include "command.h"

enum graph_node_type
{
  WORD_GN,
  IPV4_GN,
  IPV4_PREFIX_GN,
  IPV6_GN,
  IPV6_PREFIX_GN,
  VARIABLE_GN,
  RANGE_GN,
  NUMBER_GN,
  SELECTOR_GN,
  OPTION_GN,
  NUL_GN,
  END_GN
};

struct graph_node
{
  enum graph_node_type type;// data type this node matches or holds
  int is_start;             // whether this node is a start node
  vector children;          // this node's children
  struct graph_node * end;  // pointer to end for SELECTOR_GN & OPTION_GN

  char* text;       // for WORD_GN and VARIABLE_GN
  long value;       // for NUMBER_GN
  long min, max;    // for RANGE_GN

  /* cmd_element struct pointer, only valid for END_GN */
  struct cmd_element *element;
  /* used for passing arguments to command functions */
  char *arg;
};

/*
 * Adds a child to a node.
 * If the node already has the exact same child, nothing is done. This is
 * decided with cmp_node.
 *
 * @param[in] parent node
 * @param[in] child node
 * @return the new child, or the existing child if the parent already has the
 *         new child
 */
extern struct graph_node *
add_node(struct graph_node *, struct graph_node *);

/*
 * Compares two nodes for parsing equivalence.
 * Equivalence in this case means that a single user input token
 * should be able to unambiguously match one of the two nodes.
 * For example, two nodes which have all fields equal except their
 * function pointers would be considered equal.
 *
 * @param[in] first node to compare
 * @param[in] second node to compare
 * @return 1 if equal, zero otherwise.
 */
extern int
cmp_node(struct graph_node *, struct graph_node *);

/*
 * Create a new node.
 * Initializes all fields to default values and sets the node type.
 *
 * @param[in] node type
 * @return pointer to the newly allocated node
 */
extern struct graph_node *
new_node(enum graph_node_type);

/**
 * Walks a command DFA, printing structure to stdout.
 * For debugging.
 *
 * @param[in] start node of graph to walk
 * @param[in] graph depth for recursion, caller passes 0
 */
extern void
walk_graph(struct graph_node *, int);

/**
 * Returns a string representation of the given node.
 * @param[in] the node to describe
 * @param[out] the buffer to write the description into
 * @return pointer to description string
 */
extern char *
describe_node(struct graph_node *, char *, unsigned int);

/**
 * Frees the data associated with a graph_node.
 * @param[out] pointer to graph_node to free
 */
void
free_node(struct graph_node *);
#endif
