//二叉搜索树
#include <vector>
#include <stack>
#include <random>
#include <iterator>
#include <algorithm>
#include <iostream>
using namespace std;

struct TreeNode
{
    TreeNode* left;
    TreeNode* right;
    int val;
    TreeNode(int x):left(nullptr),right(nullptr),val(x){}
};

class bst{
public:
    //初始化返回根节点
    TreeNode* init(vector<int> &nums)
    {
        if(nums.empty()==true)
        {
            return nullptr;
        }
        //创建根节点
        TreeNode*root=new TreeNode(nums[0]);
        //循环遍历将数组中的数加入树中
        for(int cptime=2;cptime<=nums.size();cptime++)
        {
            add(root,nums[cptime-1]);
        }
        //存储根节点
        bst::root=root;
        //返回根节点
        return root;
    }

    //新增操作，手动调用请传入根节点，递归可传入其它节点
    bool add(TreeNode* node,int value)
    {
        if(value==node->val)
        {
            //传入相同值，不符合二叉搜索树定义
            return false;
        }
        else if(value<node->val)
        {
            if(node->left==nullptr)
            {
                TreeNode* tree=new TreeNode(value);
                node->left=tree;
                return true;
            }
            else
            {
                add(node->left,value);
                return true;
            }
        }
        else if(value>node->val)
        {
            if(node->right==nullptr)
            {
                TreeNode* tree=new TreeNode(value);
                node->right=tree;
                return true;
            }
            else
            {
                add(node->right,value);
                return true;
            }
        }
    }

    //查询操作，返回查询到节点的指针的栈，手动调用请传入根节点
    //栈底:根节点 栈顶:子节点(如果根节点或子节点不存在则栈顶为nullptr)
    stack<TreeNode*> search(TreeNode* node,int value)
    {
        stack<TreeNode*> st;
        search_algo(node,value,st);
        return st;
    }

    //删除操作，手动调用请传入根节点
    bool remove(TreeNode*node,int value)
    {
        stack<TreeNode*> s_value = search(node,value);
        bool is_root=s_value.size()==1? true:false;
        //传入不存在的根节点或者未找到相应元素执行的操作
        if(s_value.top()==nullptr)
        {
            return false;
        }
        //度为0的节点执行的操作
        else if(s_value.top()->left==nullptr && s_value.top()->right==nullptr)
        {
            if(is_root==true)
            {
                s_value.top()= nullptr;
            }
            else
            {
                TreeNode* tmp=s_value.top();
                s_value.pop();
                if(s_value.top()->left==tmp)
                {
                    delete tmp;
                    s_value.top()->left=nullptr;
                }
                else
                {
                    delete tmp;
                    s_value.top()->right=nullptr;
                }
            }
            return true;
        }
        //度为1的节点执行的操作
        else if((s_value.top()->left==nullptr) != (s_value.top()->right==nullptr))
        {
            if(is_root==true)
            {
                if(s_value.top()->left!=nullptr)
                {
                    bst::root=s_value.top()->left;
                    delete s_value.top();
                }
                else
                {
                    bst::root=s_value.top()->right;
                    delete s_value.top();
                }
            }
            else
            {
                TreeNode* grandparent=nullptr;
                TreeNode* parent=nullptr;
                TreeNode* son=nullptr;
                parent=s_value.top();
                s_value.pop();
                grandparent=s_value.top();
                if(parent->left!=nullptr)
                {
                    son=parent->left;
                }
                else
                {
                    son=parent->right;
                }
                //非法操作，直接返回
                if(parent->val==grandparent->val)
                {
                    return false;
                }
                else if(parent->val<grandparent->val)
                {
                    grandparent->left=son;
                    delete parent;
                }
                else if(parent->val>grandparent->val)
                {
                    grandparent->right=son;
                    delete parent;
                }
            }
            return true;
        }
        //度为2的节点执行的操作
        else if(s_value.top()->left!=nullptr && s_value.top()->right!=nullptr)
        {
            //生成随机数，用于后方方法的选择
            mt19937 ge(random_device{}());
            bernoulli_distribution bd(0.5);
            bool way=bd(ge);
            //获得中序遍历数组
            vector<int> vec_search;
            inorder(bst::root,vec_search);
            //将当前节点替换为左子树最大节点
            if(way==1)
            {
                int node_index= distance(vec_search.begin(),find(vec_search.begin(),vec_search.end(),s_value.top()->val));
                bst::remove(bst::root,vec_search[node_index-1]);
                s_value.top()->val=vec_search[node_index-1];
            }
            //将当前节点替换为右子树最小节点
            else
            {
                int node_index=distance(vec_search.begin(),find(vec_search.begin(),vec_search.end(),s_value.top()->val));
                bst::remove(bst::root,vec_search[node_index+1]);
                s_value.top()->val=vec_search[node_index+1];
            }
            return true;
        }
    }

    //修改节点，手动调用请传入根节点
    bool edit(TreeNode*node,int old_val,int new_val)
    {
        if(bst::remove(node,old_val)==false)
        {
            return false;
        }
        if(bst::add(node,new_val)==false)
        {
            return false;
        }
        return true;
    }

    //获取根节点
    TreeNode* Getroot()
    {
        return bst::root;
    }

    //中序遍历
    void inorder(TreeNode*node,vector<int> &nums)
    {
        if(node==nullptr)
        {
            return;
        }
        inorder(node->left,nums);
        nums.push_back(node->val);
        inorder(node->right,nums);
    }

private:
    //存储根节点
    TreeNode* root;

    //查询算法底层实现
    void search_algo(TreeNode* node,int value,stack<TreeNode*> &st)
    {
        st.push(node);
        if(node==nullptr)
        {
            return;
        }
        else if(node->val==value)
        {
            return;
        }
        else if(value<node->val)
        {
            search_algo(node->left,value,st);
            return;
        }
        else if(value>node->val)
        {
            search_algo(node->right,value,st);
            return;
        }
    }
};

int main()
{
    bst tree;
    vector<int> vc={63,39,97,29,86,99,5,61,50,67};
    tree.init(vc);
}
