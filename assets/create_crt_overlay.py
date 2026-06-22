import struct
import math

W, H = 800, 600

def make_bmp(filename, pixels):
    row_size = ((W * 3 + 3) // 4) * 4
    pixel_data = bytearray()
    for row in pixels:
        row_bytes = bytearray()
        for (r, g, b) in row:
            row_bytes += bytes([b, g, r])
        while len(row_bytes) < row_size:
            row_bytes += b'\x00'
        pixel_data += row_bytes

    file_size = 14 + 40 + len(pixel_data)
    with open(filename, 'wb') as f:
        f.write(b'BM')
        f.write(struct.pack('<I', file_size))
        f.write(struct.pack('<HH', 0, 0))
        f.write(struct.pack('<I', 14 + 40))
        f.write(struct.pack('<I', 40))
        f.write(struct.pack('<i', W))
        f.write(struct.pack('<i', H))
        f.write(struct.pack('<HH', 1, 24))
        f.write(struct.pack('<I', 0))
        f.write(struct.pack('<I', len(pixel_data)))
        f.write(struct.pack('<i', 2835))
        f.write(struct.pack('<i', 2835))
        f.write(struct.pack('<II', 0, 0))
        f.write(pixel_data)

# CRT boja (0.266, 0.412, 0.511) — multiply blend ce ovu boju naneti na ekran
CRT_R = int(0.566 * 255)  # 68
CRT_G = int(0.712 * 255)  # 105
CRT_B = int(0.811 * 255)  # 130

pixels = []
for y in range(H):
    row = []
    for x in range(W):
        nx = (x / W - 0.5) * 2.0
        ny = (y / H - 0.5) * 2.0

        dist = math.sqrt(nx * nx + ny * ny)
        vignette = max(0.0, min(1.0, (dist - 0.93) / 0.3))
        vignette = vignette ** 2.0
        vignette_factor = 1.0 - vignette * 0.15  # zatamnjenje u uglovima

        # Scan lines — svaka druga dva reda su malo tamniji
        actual_y = H - 1 - y 
        if (actual_y // 3) % 2 == 0:
            scan_factor = 0.82  # Malo tamniji red
        else:
            scan_factor = 1.0   # Normalan red

        factor = vignette_factor * scan_factor

        r = int(CRT_R * factor)
        g = int(CRT_G * factor)
        b = int(CRT_B * factor)

        row.append((r, g, b))
    pixels.append(row)

make_bmp('/Users/balkanskihaker/Desktop/Qix_final/assets/crt_overlay.bmp', pixels)
print("CRT overlay generated!")