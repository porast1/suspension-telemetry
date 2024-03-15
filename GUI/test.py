class Klasa2:
    def __init__(self, data):
        self.data = data

    def get_data(self):
        return self.data

class BaseClass:
    def __init__(self, data):
        self.klasa2 = Klasa2(data)

    def get_data_from_klasa2(self):
        return self.klasa2.get_data()

class ChildClass(BaseClass):
    def __init__(self, data):
        super().__init__(data)

    def process_data(self):
        data = self.get_data_from_klasa2()
        # Tu możesz przetworzyć dane
        return data

# Użycie
data_instance = 5
base_instance = BaseClass(data_instance)
child_instance = ChildClass(data_instance)
print(child_instance.get_data_from_klasa2())
processed_data = child_instance.process_data()