import ROOT
from array import array

# Python for FLS-1000 Ex&Em Map txt file to ROOT file.
file_path = ""
data_path = ""
txt_file_name = ""
txt_file = file_path + txt_file_name + ".txt"
root_file = data_path + txt_file_name + ".root"

# ROOT 파일 생성
# spec : wavelength & counts
# info : 기타정보
f = ROOT.TFile(root_file, "RECREATE")
tree = ROOT.TTree("spec", "FLS-1000")
itree = ROOT.TTree("info", "FLS-1000")

# 변수 정의
# wavelength : Emission wavelength
# counts     : intensity
# ex         : Excitation wavelength
wavelength = array('f', [0.0])
counts = array('f', [0.0])
ex = array('f', [0.0])

# start : Emission start wavelength
# stop  : Emission stop wavelength
# step  : Emission wavelength step
start = array('f', [0.0])
stop = array('f', [0.0])
step = array('f', [0.0])

# estart : Excitation start wavelength
# estop  : Excitation stop wavelength
# estep  : Excitation wavelength step
estep = array('f', [0.0])
estart = array('f', [0.0])
estop = array('f', [0.0])

tree.Branch("wavelength", wavelength, "wavelength/F")
tree.Branch("counts", counts, "counts/F")
tree.Branch("ex", ex, "ex/F")

itree.Branch("start", start, "start/F")
itree.Branch("stop", stop, "stop/F")
itree.Branch("step", step, "step/F")
itree.Branch("estart", estart, "estart/F")
itree.Branch("estop", estop, "estop/F")
itree.Branch("estep", estep, "estep/F")

# txt 파일 읽기
with open(txt_file) as infile:
    for line in infile:
        line = line.strip()
        indata = line.split(",")
        if indata[0] == 'Labels':
            indata.remove('Labels')
            num = len(indata) - 1
            del indata[num]
            for a in range(num):
                indata[a] = indata[a].lstrip('Ex=').rstrip('nm')
            fdata = list(map(float,indata))
            estep[0] = fdata[1]-fdata[0]
            estart[0] = fdata[0]
            estop[0] = fdata[num-1]
        if indata[0] == 'Start':
            start[0] = float(indata[1])
        if indata[0] == 'Stop':
            stop[0] = float(indata[1])
        if indata[0] == 'Step':
            step[0] = float(indata[1])
            itree.Fill()
        if not line or line[0].isalpha():   # 헤더(문자 시작) 스킵
            continue
        try:
            cols = line.split(",")
            if len(cols) >= 2:
                for b in range(num):
                    wavelength[0] = float(cols[0])
                    ex[0] = fdata[b]
                    counts[0] = float(cols[b+1])
                    tree.Fill()
        except ValueError:
            continue  # 숫자 변환 불가능한 줄은 무시

# 저장
tree.Write()
itree.Write()
f.Close()
print("{} ROOT 파일 저장 완료".format(txt_file_name))

