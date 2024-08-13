import requests
import pandas as pd


def get_atomic_dataframe(url: str, start_index: int) -> pd.DataFrame:
    """
    2020 version
    data format (fortran)
    a1,i3,i5,i5,i5,1x,a3,a4,1x,f14.6,f12.6,f13.5,1x,f10.5,1x,a2,f13.5,f11.5,1x,i3,1x,f13.6,f12.6
    unit: MeV, amu
    """

    data_response = requests.get(url)
    lines = data_response.text.splitlines()

    data = []
    for line in lines[start_index:]:
        record = {
            "cc": line[0:1].strip(),
            "NZ": int(line[1:4].strip()),
            "N": int(line[4:9].strip()),
            "Z": int(line[9:14].strip()),
            "A": int(line[14:19].strip()),
            "El": line[20:23].strip(),
            "o": line[23:27].strip(),
            "mass_excess": float(line[28:42].strip().replace("#", "")) * 1.0e-3,
            "unc_mex": float(line[42:54].strip().replace("#", "")) * 1.0e-3,
            "binding": float(line[54:67].strip().replace("#", "")) * 1.0e-3,
            "unc_bid": float(line[68:78].strip().replace("#", "")) * 1.0e-3,
            # "B": line[79:81].strip(),
            # "beta": line[81:94].strip(),
            # "unc_beta": line[94:105].strip(),
            "atomic_mass": float(
                line[106:123].strip().replace(" ", "").replace("#", "")
            )
            * 1.0e-6,
            "unc_mass": float(line[123:135].strip().replace("#", "")) * 1.0e-6,
        }
        data.append(record)
    return pd.DataFrame(data)


if __name__ == "__main__":
    """
    Get Mass Database
    please input the mass_url and start_index number correctly if new mass table are released
    """
    mass_url = "https://www-nds.iaea.org/amdc/ame2020/mass_1.mas20.txt"
    df = get_atomic_dataframe(url=mass_url, start_index=36)
    # print(df.head())

    z_array = df["Z"].to_numpy()
    a_array = df["A"].to_numpy()
    mass_array = df["atomic_mass"].to_numpy()

    """
    Modify the Mass.h
    """
    header_file = "Mass.h"
    with open(header_file, "r") as file:
        header_content = file.read()

    start_marker = "    arr["  # start str
    end_marker = "    return arr;"  # end str
    start_index = header_content.find(start_marker)
    end_index = (
        header_content.find(end_marker, start_index) if start_index != -1 else -1
    )
    # print(header_content[:start_index])
    # print(header_content[end_index:])

    new_content = ""
    for z, a, mass in zip(z_array, a_array, mass_array):
        new_content += f"    arr[{z}][{a}] = {mass:.12f};\n"
    new_content += "\n"

    if start_index != -1 and end_index != -1:
        new_header_content = (
            header_content[:start_index] + new_content + header_content[end_index:]
        )
    else:
        raise ValueError(
            f'invalid {header_file} file or start_marker "{start_marker}" end_marker"{end_marker}"'
        )

    with open(header_file, "w") as file:
        file.write(new_header_content)

    print(f"updated {header_file}")
