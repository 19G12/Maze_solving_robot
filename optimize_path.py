path = "LBLLLBFBLLBFLL"

opt_path = {
    'LBL': 'F',
    'LBR': 'B',
    'RBL': 'B',
    'LBF': 'R',
    'RBR': 'F',
    'RBF': 'L',
    'FBL': 'R',
    'FBR': 'L',
    'FBF': 'B'
}

def sliding_window(path):
    ws = 3
    set = 1
    while(set):
        set = 0
        for ref in range(len(path) - 3):
            window_value = path[ref:ws + ref]
            
            if window_value in opt_path:
                path = path.replace(window_value, opt_path[window_value])
                set = 1
    return path

print(sliding_window(path))