import pyrealsense2 as rs  # Setting an alias for the library


def print_image_as_text(depth):
    """
    Print a simple text-based representation of the image.


    This happens by breaking it into 10x20 pixel regions
    and approximating the coverage of pixels within one meter
    """
    output = ""
    coverage = [0]*64
    for y in range(480):
        for x in range(640):
            dist = depth.get_distance(x, y)
            if 0 < dist and dist < 1:
                coverage[int(x/10)] += 1

        if y%20 is 19:
            line = ""
            for c in coverage:
                line += " .:nhBXWW"[int(c/25)]
            coverage = [0]*64
            output += line + '\n'
            # print(line)
    print(output)
    print("="*60)


pipeline = rs.pipeline()
pipeline.start()

try:
    # Do stuff
    while 1:
        # Wait for next frame (allows support for different refresh rates)
        rgb_frame = pipeline.wait_for_frames()
        depth_frame = rgb_frame.get_depth_frame()
        if not depth_frame: continue  # Need?

        # Print as text
        print_image_as_text(depth_frame)
finally:
    pipeline.stop()