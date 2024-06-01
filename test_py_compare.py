import random
import cqueue
import time

# build test data
repeat = 100
chrs = 'abcdefghijklmnopqrstuvwxyz!@#$%^&*()'

rand_size = 100
rands = []

for _ in range(rand_size):
    if random.random() > 0.5:
        rands.append(random.randint(0, 100))
    else:
        s = ""
        for _ in range(random.randint(0, 100)):
            s = s + random.choice(chrs)
        rands.append(s)


# python queue
class Queue(list):
    def push(self, obj):
        self.insert(0, obj)

    def pop(self):
        if len(self) <= 0:
            return None
        return super(Queue, self).pop(0)

    def size(self):
        return len(self)


def check_logic_right(queue):
    # case 1 checker
    for _ in range(repeat):
        for idx in range(rand_size):
            a = rands[idx]
            queue.push(a)
            if a != queue.pop():
                return False
    # case 2 checker
    for _ in range(repeat):
        counter = 0
        a = []
        for idx in range(rand_size):
            a.append(rands[idx])
            queue.push(rands[idx])
            if counter >= 100:
                while queue.size() > 0:
                    if queue_size != len(a):
                        return False
                    if a.pop(0) != queue.pop():
                        return False
            counter += 1
    return True


def do_queue_feature(queue):
    # case 1: push and pop
    start = time.perf_counter_ns()
    for _ in range(repeat):
        for idx in range(rand_size):
            queue.push(rands[idx])
            queue.pop()
    print(f"[push and pop] {(time.perf_counter_ns() - start) / 1000.0 / 1000.0}ms")
    # case 2: pop every 100 push
    start = time.perf_counter_ns()
    for _ in range(repeat):
        counter = 0
        for idx in range(rand_size):
            queue.push(rands[idx])
            if counter >= 100:
                while queue.size() > 0:
                    queue.size()
                    queue.pop()
            counter += 1
    print(f"[pop every 100 push] {(time.perf_counter_ns() - start) / 1000.0 / 1000.0}ms")


if __name__ == "__main__":
    # python queue
    print("============== python queue ================")
    queue = Queue()
    do_queue_feature(queue)
    # cqueue
    print("============== cqueue ================")
    if check_logic_right(queue):
        print("[check logic] Success!")
        queue = cqueue.queue()
        do_queue_feature(queue)
    else:
        print("[check logic] Failed!")
