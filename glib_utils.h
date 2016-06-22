#ifndef __G_NODE_UTILS_H__
#define __G_NODE_UTILS_H__

#include<glib.h>

/*some other stuff*/
typedef struct _NodeInfo {
	glong id;			/*unique id for each node; auto-increment this*/
	glong data;			/*data of the node*/
	gdouble distance;	/*distabce of the node from parent; currently unused*/
} NodeInfo;

/*function definitions GArray*/
void g_array_print(GArray *array);
GArray *g_array_intersection(GArray *a, GArray *b);
GArray *g_array_difference(GArray *a, GArray *b);
GArray *g_array_join(GArray *a, GArray *b);
GArray *g_array_clone(GArray *a);
GArray *g_array_reverse(GArray *a);

/*function definitions GNAryTree*/

gboolean g_node_free_func_struct(GNode *node, gpointer data);

GNode* g_node_common_ancestor(GNode *node1, GNode *node2);

/*indexing*/
GNode* g_node_index_struct(GNode *root, glong offset);
gboolean g_node_index_func_struct(GNode *node, gpointer data);
/*********/

/*read newick tree from a file*/
GNode* g_node_read_newick(FILE *lfile);

GNode* _g_node_read_newick_node(FILE *file);
gchar* _g_node_read_newick_node_data(FILE *file);
NodeInfo* _g_node_read_newick_nodeinfo(FILE *file);
/*********************/

/*write a newick tree to file*/
GNode *g_node_write_newick(GNode *root, FILE *file); /*returns NULL when successfull*/
GNode *_g_node_write_newick_nodeinfo(GNode *node, FILE *file);
/***********************/

gboolean g_node_get_data (GNode *node, gpointer data);
gboolean g_node_get_data_struct(GNode *node, gpointer data);
gboolean g_node_get_node_struct(GNode *node, gpointer data);

glong g_node_path_to_root_data(GNode *node, GArray *path);
glong g_node_path_to_root_position(GNode *node, GArray *path);
glong g_node_no_nodes_on_left(GNode *node);
void g_node_path_identifiers(GNode *node, GArray *array);

gboolean g_node_print_struct(GNode *node, gpointer data);
gboolean g_node_find_func_struct(GNode *node, gpointer data);
GNode* g_node_find_struct (GNode *root, GTraverseType order, GTraverseFlags flags, gpointer data);
GArray *g_node_path_to_root_struct_id(GNode *node);

/* GHash utils */
void g_hash_table_foreach_print(gpointer key, gpointer value, gpointer user_data);

#endif /*__G_NODE_UTILS_H__*/
