import pyrealsense2 as rs
import threading
import pdb

def pullFrames(color_queue, depth_queue):
    pipeline = rs.pipeline()
    pipeline.start()

    try:
        while 1:
            rgb_frame = pipeline.wait_for_frames()
            depth_frame = rgb_frame.get_depth_frame()
            if not depth_frame: continue

            if(len(color_queue) < 2000000):
                print("loop1")
                color_queue.append(rgb_frame)
            if(len(depth_queue) < 2000000):
                print("loop2")
                depth_queue.append(depth_frame)

    finally:
        pipeline.stop()

def getImage(color_queue, depth_queue):
    if((len(color_queue) >= 1) and (len(depth_queue) >= 1)):
        pair = (color_queue.pop(), depth_queue.pop())
        return pair

def processImage(color_queue, depth_queue):
    while 1:
        getImage(color_queue, depth_queue)
        print("Processed Image")

color_queue = []
depth_queue = []

camera_thread = threading.Thread(target=pullFrames, args=(color_queue, depth_queue,))
processing_thread = threading.Thread(target=processImage, args=(color_queue, depth_queue,))

camera_thread.start()
processing_thread.start()
