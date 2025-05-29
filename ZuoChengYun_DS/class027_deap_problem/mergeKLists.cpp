// https://www.nowcoder.com/practice/65cfde9e5b9b4cf2b6bafa5f3ef33fa6

/**
 * struct ListNode {
 *  int val;
 *  struct ListNode *next;
 *  ListNode(int x) : val(x), next(nullptr) {}
 * };
 */

#include <queue>
#include <vector>
using namespace std;

class Solution 
{
  public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     *
     * @param lists ListNode类vector
     * @return ListNode类
     */
    ListNode* mergeKLists(vector<ListNode*>& lists) // 容器中都是每个链表的头指针
    {

        //检查边缘情况
        int count = 0;
        for(ListNode *cur : lists)
        {
            if(cur == nullptr) {count++;}
        }
        if(count == lists.size()) // 全是NULL
        {
            return nullptr;//结束
        }

        //创立小根堆
        auto cmp = [](ListNode *a, ListNode *b){
            return a->val > b->val;
        };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> queue(cmp);

        //把所有链表头拿到堆里面
        for(ListNode *cur : lists)
        {
            if(cur != nullptr)
            {
                queue.push(cur);
            }
        }

        //创立h,p
        ListNode *h = nullptr;
        ListNode *p = nullptr;

        //持续地取，串，放。直到堆空
        h = queue.top();
        while(queue.size() > 0)
        {
            if(queue.empty() == false)
            {
                p = queue.top();
                queue.pop();
            }

            if(p->next != nullptr)
            {
                queue.push(p->next);
            }
            
            if(queue.empty() == false)
            {
                p->next = queue.top();
                p = p->next;
            }
        }

        return h;
    }
};
