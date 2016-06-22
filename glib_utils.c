#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "glib_utils.h"


/*
GArray utilities
*/

void g_array_print(GArray *array) {
  long int i;
  printf("\n");
  for(i=0;i<array->len;i++)
	printf("%ld ",g_array_index(array,long,i));
}

/* find intersection of two arrays
it is assumed that each array contains unique elements*/
GArray *g_array_intersection(GArray *a, GArray *b) {

glong i,j;
GArray *intersect = g_array_new(FALSE,FALSE,sizeof(glong));
        for(i=0; i< a->len; i++) {
                for(j=0; j< b->len; j++) {
                        if(g_array_index(a,gpointer,i) == g_array_index(b,gpointer,j)) {
                                g_array_append_val(intersect, g_array_index(a,gpointer,i));
                                break;
                        }
                }
        }

return(intersect);

}

/*find difference between two arrays a-b
NOTE (a-b) = (b-a)*/
/*NOTE: its possible to make this faster*/
GArray *g_array_difference(GArray *a, GArray *b) {

  glong i,j, diff_count;
  GArray *difference = g_array_new(FALSE,FALSE,sizeof(glong));
  for(i=0; i< a->len; i++) {
  /*take an element of a, check if its present in b. if not then add it to difference.*/
        diff_count = 0;
        for(j=0; j< b->len; j++) 
                if(g_array_index(a,gpointer,i) != g_array_index(b,gpointer,j))
                      diff_count++;

        /*add element if no match found*/
        if(diff_count == b->len)
                g_array_append_val(difference, g_array_index(a,gpointer,i));
  } 

  return(difference);

}

/*join two arrays*/
GArray *g_array_join(GArray *a, GArray *b) {
  GArray *join = g_array_new(FALSE,FALSE,sizeof(glong));
  glong i;
  for(i=0;i<a->len;i++)
    g_array_append_val(join, g_array_index(a,gpointer,i));

  for(i=0;i<b->len;i++)
    g_array_append_val(join, g_array_index(b,gpointer,i));

  return join;
  
}

/*contents of a are copied to b
use insert instead of append, this helps if b is sized array*/
GArray *g_array_clone(GArray *a) {
GArray *b = g_array_new(FALSE,FALSE,sizeof(glong));
	if(b->len!=0) {
		perror("In g_array_clone: the destination array must be of size 0.");
		exit(0);
	}
        int i;
        for(i=0;i<a->len;i++) {
                g_array_insert_val(b, i, g_array_index(a, gpointer,i));		
        }

        return(b);
}

/*reverse an array*/
GArray *g_array_reverse(GArray *a) {
GArray *b = g_array_new(FALSE,FALSE,sizeof(glong));
	if(b->len!=0) {
		perror("In g_array_reverse: the destination array must be of size 0.");
		exit(0);
	}
	
    gint i;
	for(i=0;i<a->len;i++)
		g_array_prepend_val(b, g_array_index(a, gpointer,i));
        
	return(b);
}


/*
 * GNAryTree utilities
 */

/* 
get common ancestor
make sure that if node1==node2 then return the node1 (or node2) itself
*/
GNode *g_node_common_ancestor(GNode *node1, GNode *node2) {
	GNode *ancestor;

	if(G_NODE_IS_ROOT(node1))
		ancestor = node1;
	else if(G_NODE_IS_ROOT(node2))
		ancestor = node2;
	else if(node1==node2)
		ancestor = node1;
	else {
		ancestor = node1;
		while(!g_node_is_ancestor(ancestor, node2))
			ancestor = ancestor->parent;
	}

	return ancestor;
}


gboolean g_node_free_func_struct(GNode *node, gpointer data) {
	
	g_free(((NodeInfo*)node->data));
	
	return FALSE;  	
}

gboolean g_node_print_struct(GNode *node, gpointer data) {
	
	printf("\nTree Node=> id: %ld data: %ld", (glong)((NodeInfo*)node->data)->id, (glong)((NodeInfo*)node->data)->data);
	return FALSE;
}

/*get data of the given node 
for using with g_node_traverse*/
gboolean g_node_get_data(GNode *node, gpointer data) {
	glong i;
	i = (glong) node->data;	
	g_array_append_val((GArray*)data, i);
	return FALSE;
}

gboolean g_node_get_data_struct(GNode *node, gpointer data) {
	glong i;
	i = (glong) ((NodeInfo*)node->data)->data;	
	g_array_append_val((GArray*)data, i);
	return FALSE;
}

/*get pointers to all nodes*/
gboolean g_node_get_node_struct(GNode *node, gpointer data) {
	g_array_append_val((GArray*)data, node);
	return FALSE;
}

GNode* g_node_find_struct(GNode *root, GTraverseType order, GTraverseFlags flags, gpointer data) {
  gpointer d[2];

  g_return_val_if_fail (root != NULL, NULL);
  g_return_val_if_fail (order <= G_LEVEL_ORDER, NULL);
  g_return_val_if_fail (flags <= G_TRAVERSE_MASK, NULL);

  d[0] = data;
  d[1] = NULL;

  g_node_traverse (root, order, flags, -1, g_node_find_func_struct, d);

  return d[1];
}


gboolean g_node_find_func_struct(GNode *node, gpointer data) {
	
	/*printf("\nSearch reached %ld: %ld", ((NodeInfo*)node->data)->id, ((NodeInfo*)node->data)->data);*/
	
  	gpointer *d = data;
	
	if ((glong)*d!=(glong)((NodeInfo*)node->data)->data)
    		return FALSE;

  	*(++d) = node;

  	return TRUE;
}

GNode* g_node_index_struct(GNode *root, glong offset) {
  glong off = offset;	
      
  g_node_traverse(root, G_PRE_ORDER, G_TRAVERSE_ALL, -1, g_node_index_func_struct, &off);	
	
  return root;
}

gboolean g_node_index_func_struct(GNode *node, gpointer data) {
	
	((NodeInfo*)node->data)->id = (*(glong*)data)++;
	
	return FALSE;  	
}

/*get path to the root using the structure NodeInfo*/
GArray *g_node_path_to_root_struct_id(GNode *node) {
	GArray *array = g_array_new(FALSE, FALSE, sizeof(glong));
	GNode *current;
	current = node;
	glong j;
	while(current!=NULL) {
		j = ((NodeInfo*)current->data)->id;		
		g_array_append_val(array,j);
		current = current->parent;
	}
	return array;
}

/* get path to the root*/
glong g_node_path_to_root_data(GNode *node, GArray *path) {
	GNode *par;
	
	/*first append own data*/
	g_array_append_val(path, node->data);
	
	par = node->parent;
	/*parent of the root node is NULL*/
	while(par!=NULL) {		
		g_array_append_val(path, par->data);
		par = par->parent;	
	}
	return(1);
}

/*this function is for getting respective position of the nodes along the path starting at the given node*/
glong g_node_path_to_root_position(GNode *node, GArray *path) {
	GNode *par;
	par = node;
	gint pos;
	/*parent of the root node is NULL*/
	while(par->parent!=NULL) {
		pos = g_node_child_position(par->parent, par);
		g_array_append_val(path, pos);
		par = par->parent;	
	}

	/*append root node as -1*/
	glong rn = -1;
	g_array_append_val(path,rn);
	return(1);
}

/*count number of nodes on left of any given node (when the tree is cut at its parent node)
its useful to get node identifiers in depth first manner*/
glong g_node_no_nodes_on_left(GNode *node) {
	glong i, no_nodes, position;
	/*first get the root_child_parent*/
	if(node->parent==NULL) {
		perror("\nIn g_node_no_nodes_on_left: cant calculate for root node.");	
		exit(0);
	}
		
	position = g_node_child_position(node->parent, node);

	no_nodes=0;
	for(i=0;i<position;i++) {
		no_nodes += g_node_n_nodes(g_node_nth_child(node->parent,i), G_TRAVERSE_ALL);
	}

	return no_nodes;

}

/*give unique identifiers to the nodes in a path
in a depth first manner starting at the top of the tree*/
void g_node_path_identifiers(GNode *node, GArray *array) {
	glong i, no_nodes_left;

	/*get a pointer to the topmost non root parent*/
	GNode *par=node;
	while(!G_NODE_IS_ROOT(par->parent)) {
		par = par->parent;
	}

	/*append 0 for root node*/
	i=0;
	g_array_append_val(array,i);

	/*check if node is the root*/
	if(G_NODE_IS_ROOT(node)) {
		printf("\nNo path for root node...");
		exit(0);		
	}


	/*append id of topmost non root parent*/
	no_nodes_left = 0;
	no_nodes_left =  1 + g_node_no_nodes_on_left(par);
	g_array_append_val(array,no_nodes_left);

	/*get the path to root of the node
	this makes it more efficient(?) to traverse the tree*/
	GArray *a = g_array_new(FALSE, FALSE, sizeof(long));
	g_node_path_to_root_position(node, a);
	GArray *path=g_array_reverse(a);

	/*the iterator starts at 2 as we already have first two ids*/
	for(i=2;i<path->len;i++) {		
		/*change par to appropriate node*/
		par = g_node_nth_child(par,g_array_index(path,int,i));
		
		no_nodes_left += 1 + g_node_no_nodes_on_left(par);
		g_array_append_val(array,no_nodes_left);
	}	

	/*free path*/
	g_array_free(a, TRUE);
	g_array_free(path, TRUE);

}

/*read newick format from a file
currently only long is supported as the node names
returns pointer to the root node*/
GNode* g_node_read_newick(FILE *file) {
	
	if(file == NULL) {
		printf("In g_node_read_lewick: Can not read file.");
		exit(0);
	}
	
	glong ch;	
	GNode *root=NULL;	

	ch = fgetc(file);
  	if(ch == '(' ) {
    	root = _g_node_read_newick_node(file);
    	if(!root) {
			printf("Out of memory: Failed to read classification taxonomy.");
			exit(0);
		}      		
  	}
  
	ch = fgetc(file);
	if(ch != ';') {
    	printf("File format error: missing ';' at the end.");
    	exit(0);
	}
  
	return(root);
		
}


/* supporting functions for g_node_read_newick */

/* read a node. its a recursive function */
GNode* _g_node_read_newick_node(FILE *file) {
	gchar ch;	
	NodeInfo *nc;
	GNode *node, *child;	
	
	node = g_node_new(NULL); /* node name will come later */
	
	do {
		ch = fgetc(file);
    	if(ch == '(') {
			/*here comes the recursion*/
      		child = _g_node_read_newick_node(file);			
			
      		if(!child) {
				printf("Cannot read node");
				exit(0);
			}      	
			child=g_node_append(node,child);			
    	}
    	else {
			/*read it as a leaf node*/
      		ungetc(ch, file);
      		nc = _g_node_read_newick_nodeinfo(file);			
			
      		child = g_node_new(nc);
			child = g_node_append(node,child);			
    	}
    
    	ch = fgetc(file);
		
	} while(ch == ',');
	
	if(ch == ')') {
		/*now its time to read node's information*/
    	node->data = (gpointer)_g_node_read_newick_nodeinfo(file);
		
  	}
  	else {
    	printf("Invalid file format: no closing brace");
    	exit(0);
  	}
	return node;
}

/*reads nodeinfo in "name:distance" format*/
NodeInfo *_g_node_read_newick_nodeinfo(FILE *file) {
	NodeInfo *n;
	gchar *name;
	gint ch;
	
	n = g_try_new(NodeInfo,1);
	
	if(n==NULL) {
		printf("\nIn _g_node_read_newick_nodeinfo: Can not allocate memory for structure NodeInfo.");
		exit(0);
	}
	
	
	/*read node data*/
	/*n->data = (gpointer)_g_node_read_newick_node_data(file);*/
	name = _g_node_read_newick_node_data(file);
	/*n->data = g_malloc(sizeof(glong));*/
	sscanf(name,"%ld",&n->data);
	
	/*read distance if specified*/
	ch = fgetc(file);
	if(ch==':')
			fscanf(file,"%lf",(gdouble*)&n->distance);
	else {
			ungetc(ch,file);
			n->distance = 0.0;
	}
	
	/*free unnecessary memory*/
	g_free(name);
	
	return n;
}

/*read node data (name) as a string*/
gchar* _g_node_read_newick_node_data(FILE *file) {
	gchar *data;
	gchar ch;
	gint i=0;
	
	data = g_malloc(sizeof(gchar)*32);
	while( (ch = fgetc(file)) != EOF) {
		if(isspace(ch) || strchr(" ()[]\':;,", ch)) {
			ungetc(ch, file);
			data[i++] = 0;
			return data;
		}
		else {
			data[i++] = (gchar)ch;
			if(i>=32) {
				printf("Node name exceeds limit of 32bytes.");
				exit(0);
			}
		}	  
			
    }
	data[i] = 0;
	return data;
}


/*end of supporting functions*/

/*
Kaustubh Patil: 07-04-2008
write a tree into file 'file' in newick format
WARNING!!! this function destroyed the tree
*/
GNode *g_node_write_newick(GNode *root, FILE *file) {

	GNode *current;	

	current = root;		
		
	/*print data and then pass control to appropriate node*/
	if(current==NULL) {		
		fprintf(file,";\n");
		return current;
	}
	else if(G_NODE_IS_ROOT(current)) {
		if(g_node_n_children(current)!=0) {
			fprintf(file,"(");
			current = g_node_first_child(current);
		}
		else
			current = _g_node_write_newick_nodeinfo(current, file);
		
		g_node_write_newick(current, file);	
	}
	else if(G_NODE_IS_LEAF(current)) {
		/*printf("\nI am leaf: %ld",(glong)((NodeInfo*)current->data)->data);*/
		current = _g_node_write_newick_nodeinfo(current, file);
		g_node_write_newick(current, file);			
	}
	else {
		fprintf(file,"(");	
		current = g_node_nth_child(current, 0);
		/*printf("\nRecusion with: %ld",(glong)((NodeInfo*)current->data)->data);*/
		g_node_write_newick(current, file);	
	}
	
	
	return current;

}

/*supporting functions for writing newick tree*/

/*returns next sibling if exists otherwise returns parent
WARNING!!! it destroys the passed node*/
GNode *_g_node_write_newick_nodeinfo(GNode *node, FILE *file) {
	
	GNode *ret;
	
	fprintf(file,"%ld",(glong)((NodeInfo*)node->data)->data);
	if(((NodeInfo*)node->data)->distance!=0) 
		fprintf(file,":%lf",((NodeInfo*)node->data)->distance);
	
	if(g_node_next_sibling(node)!=NULL) {
		fprintf(file,",");
		ret = g_node_next_sibling(node);
	}
	else {		
		ret = node->parent;
		if(ret!=NULL) /*this takes care of the root node*/
			fprintf(file,")");
	}
	
	g_free((NodeInfo*)node->data);
	g_node_destroy(node);
	
	return ret;
	
}

/* GHash utils */

/* below user_data must be a string defining the printf format */
void 
g_hash_table_foreach_print(gpointer key, gpointer value, gpointer user_data) {
	printf(user_data,key,value);
	printf("\n");
}
