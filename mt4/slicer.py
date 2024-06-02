import os
import cv2
import numpy as np
import sys
import json


def img_to_contours(img_path):
    image = cv2.imread(img_path, 0)
    image = cv2.GaussianBlur(image, (3, 3), 1.5, 1.5)
    edges = cv2.Canny(image, 50, 150)
    cv2.imshow('edges', edges)

    contours, _ = cv2.findContours(edges, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    lines = np.concatenate([np.stack([contour[:-1, 0], contour[1:, 0]], axis=1) for contour in contours])
    print(f"Кол-во сегментов картинки: {lines.shape}")

    segments = []
    for line in lines:
        seg = []
        for i, point in enumerate(line):
            seg.append((f"x{i+1}", float(point[0])))
            seg.append((f"y{i+1}", float(point[1])))
        segments.append(dict(seg))

    print(segments[0])
    img_name = img_path.split("\\")[-1].split('.')[0]
    with open(f'D:\.dev\m_threads\mt4\segmets_from_imgs\{img_name}.json', 'w') as f:
        json.dump(segments, f, indent=4)

    test = np.zeros(edges.shape + (3,), dtype=np.uint8)
    for i in range(len(lines)):
        color = np.random.uniform(0, 255, size=3)
        cv2.line(test, lines[i][0], lines[i][1], color, 1)

    cv2.imshow('test', test)
    cv2.waitKey()


def main():
    images_dir = "D:\.dev\m_threads\mt4\images"
    img_paths = [os.path.join(images_dir, f) for f in os.listdir(images_dir)]
    print([(i, f.split("\\")[-1]) for i, f in zip(range(len(img_paths)), img_paths)])
    ind = abs(int(input()))

    img_to_contours(img_paths[min(ind, len(img_paths) - 1)])


if __name__ == "__main__":
    main()
