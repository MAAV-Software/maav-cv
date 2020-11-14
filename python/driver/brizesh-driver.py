import pyrealsense2 as rs
from threading import Thread
from threading import Lock
import pdb

def process(rgb_frame,depth_frame):
    rgb_frame = rgb_frame
    depth_frame = depth_frame

    pipeline = rs.pipeline()
    pipeline.start()
    color_queue = []
    depth_queue = []
    try:
        while true:
            rgb_frame = pipeline.wait_for_frames()
            depth_frame = rgb_frame.get_depth_frame()
            # if not depth_frame: continue
            # if (len(color_queue)<1000000 and len(depth_queue)< 1000000):
            #     color_queue.append(rgb_frame)
            #     depth_queue.append(depth_frame)

# def benchmark():
#     while 1:
#         mutex = Lock
#         mutex.acquire

#         while (len(color_queue)!=0):
#             rgb_frame = color_queue[0]
#             depth_frame = depth_queue[0]
#             color_queue.remove(0)
#             depth_queue.remove(0)
#             mutex.release
#             process(rgb_frame,depth_frame)



#     finally:
#         pipeline.stop()