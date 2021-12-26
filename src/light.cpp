#include <light.h>

void useLight(ShaderProgram &prgm, const dirLight &dir)
{
    prgm.setBool("en_light0", true);
    prgm.setVec3("light0_dir", dir.direction);
    prgm.setVec3("light0.ambient", dir.ambient);
    prgm.setVec3("light0.diffuse", dir.diffuse);
    prgm.setVec3("light0.specular", dir.specular);
}
void useLight(ShaderProgram &prgm, const pointLight &point)
{
    prgm.setBool("en_light1", true);
    prgm.setVec3("light1_pos", point.position);
    prgm.setVec3("light1.ambient", point.ambient);
    prgm.setVec3("light1.diffuse", point.diffuse);
    prgm.setVec3("light1.specular", point.specular);
    prgm.setFloat("light1.constant", point.constant);
    prgm.setFloat("light1.linear", point.linear);
    prgm.setFloat("light1.constant", point.quadratic);
}