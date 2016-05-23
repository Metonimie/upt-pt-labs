#include <stdio.h>
#include <stdlib.h>

typedef struct _bst * Bnode;

struct _bst {
  Bnode left;
  Bnode right;
  float data;
};

Bnode root;

Bnode init(float data) {
  Bnode node = malloc(sizeof(struct _bst));
  node->data = data;
  node->left = NULL;
  node->right = NULL;

  return node;
};

void insert(Bnode node, float data) {
  if (data >= node->data && node->right) {
    insert(node->right, data);
  } else if (data < node->data && node->left) {
    insert(node->left, data);
  } else {
    Bnode temp = malloc(sizeof(struct _bst));
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    if (data > node->data) {
      node->right = temp;
    } else if (data < node->data) {
      node->left= temp;
    } else {
      free(temp);
      printf("%f is already in node!\n", data);
    }
  }
}

void print_tree(Bnode root) {
  if (root) {
    printf("%f - ", root->data);
    if (root->right) {
      printf(" R: %f ", root->right->data);
    }
    if (root->left) {
      printf(" L: %f ", root->left->data);
    }
    printf("\n");
  } else {
    return;
  }
  print_tree(root->right);
  print_tree(root->left);
}

int main() {
  root = init(10);
  insert(root, 5);
  insert(root, 14.5);
  insert(root, 13);
  insert(root, 1);
  insert(root, 1);
  insert(root, 16);
  insert(root, 19);
  print_tree(root);
  return 0;
}
