<script setup lang="ts">
import { ref } from "vue";
import AsideMenu from "@/views/layout/components/AsideMenuComponent.vue";
import Header from "@/views/layout/components/HeaderComponent.vue";
const asideCollapse = ref(false);
const asideWidth = ref("200px");

const toggleAsideCollapse = () => {
  asideCollapse.value = !asideCollapse.value;
  asideWidth.value = asideCollapse.value ? "64px" : "200px";
}
</script>

<template>
  <div class="common-layout">
    <el-container>
      <el-aside :width="asideWidth">
        <AsideMenu :collapse="asideCollapse" />
      </el-aside>
      <el-container>
        <el-header>
          <Header :asideCollapse="asideCollapse" @toggleAsideCollapse="toggleAsideCollapse" />
        </el-header>
        <el-main><RouterView /></el-main>
      </el-container>
    </el-container>
  </div>
</template>

<style lang="scss" scoped>
.common-layout {
  height: 100%;
  .el-header {
    box-shadow: 0 1px 4px rgba(0, 0, 0, .08);
  }
  .el-container {
    height: 100%;
  }
}
</style>